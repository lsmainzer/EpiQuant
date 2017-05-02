import scala.io.Source
import scala.annotation.tailrec
import scala.math.sqrt
import breeze.numerics._
import breeze.linalg.DenseVector
import breeze.linalg.DenseMatrix
import breeze.stats.hypothesis._
import org.apache.spark.ml.regression._
import org.apache.spark._
import org.apache.spark.sql._
import org.apache.spark.broadcast._
import org.apache.commons.math3.distribution.TDistribution

case class RegressionOutput(SNP_name: String, phenotype_name: String, coefficient: Double,
                            intercept: Double, p_value: Double, SNP_values: Vector[Double]
                           ) 

case class ModelEntry(SNP_name: String, coefficient: Double, p_value: Double)

object GWAS {
  
  /*
   * These are the only mutable variables in the code; they store the parameters of the application,
   *   which are set to a default, but can be changed from the users input
   */
  var threshold = 0.05
  
  def parseInputFile(input: String): Vector[(String, Vector[Double])] = {
    /*
     * Note: the R^2 result of breeze's linear regression is known to be completely inaccurate (luckily,
     *   we don't need it for this analysis)
     * 
     * This code assumes the order of each data entry (row) is the same between the
     *   input SNP file and the input Phenotype file 
     */
    
    // Open file handle
    val buffSource = Source.fromFile(input)
    
    // Grab the header line (and split it)
    val header: Vector[String] = buffSource.getLines.next.split("\t").toVector
    // Grab all of the other lines (and split them)
    val lines: Vector[Vector[String]] =
      buffSource.getLines.toVector.map(line => line.split("\t").toVector)
    
    buffSource.close()
    
    /*
     * The "columns" variable is a collection of collections: for each column, it has a collection
     *   of the values for that column
     */
    // For each column ...
    val columns: IndexedSeq[Vector[Double]] = for (i <- 1 until header.length) yield {
      // ... Go through each line and return the value for that column
      lines.map(x => x(i).toDouble) 
    }
   
    // Turn columns into collection of Tuples with (ColumnName, ValueList)
    val names_values: IndexedSeq[(String, Vector[Double])] =
      // i + 1 in header because ID column gone in "columns" but still present in "header"
      for (i <- 0 until columns.length) yield (header(i + 1), columns(i))
      
    return names_values.toVector 
  }
  
  def createPairwiseList(initialCols: IndexedSeq[(String, Vector[Double])]
      ): IndexedSeq[(String, String)] = {
    // Creates a list of all pairwise combinations of each column (names only, does not
    //   actually compute the new values: that will be done per partition)
    for (i <- 0 until initialCols.length; j <- i + 1 until initialCols.length) yield {
      (initialCols(i)._1, initialCols(j)._1)
    }
  }
  
  def createPairwiseCombinations(
      inputTuple: (String, String),
      broadcastTable: Broadcast[Map[String,Vector[Double]]]
      ): (String, Vector[Double]) = {
    // Given a Tuple with two column names, return a tuple of their values joined in
    //   the following way: ("Col1*Col2" , newValues)
    val newColName = inputTuple._1 + "*" + inputTuple._2
    val column1 = broadcastTable.value(inputTuple._1)
    val column2 = broadcastTable.value(inputTuple._2)
    
    val newColumn = for (i <- 0 until column1.length) yield { column1(i) * column2(i) }
    (newColName, newColumn.toVector)
  }
  
  def performSimpleLinearRegression(
      SNP: (String, Vector[Double]),
      Pheno: (String, Vector[Double])
      ): RegressionOutput = {
    // Output is (SNP name, Phenotype name, slope, intercept, p-value, SNP_values)    
    
    val Xvalues = DenseVector(SNP._2.toArray)
    val Yvalues = DenseVector(Pheno._2.toArray)

    // This is just a vector of ones (used to get the intercept coefficient when modeled
    val interceptVector = DenseVector.fill(Xvalues.length){1.0}
    
    // This matrix is created only because the independent variables have to be passed to breeze in a matrix
    val Xmatrix = DenseMatrix(Xvalues, interceptVector).t
    
    // Model will give us the coefficient and intercept
    val model = breeze.stats.regression.leastSquares.matrixVector(Xmatrix, Yvalues)

    // Model: Y = intercept + slope*X
    val slope = model.coefficients(0)
    val intercept = model.coefficients(1)
    val degreesOfFreedom = Xvalues.length - 2

    val predictedY = Xvalues.map(x => x*slope + intercept)
    val residuals = Yvalues - predictedY
    val sumOfSquaredResiduals = residuals.map(x => x * x).reduce((x,y) => x + y)
    
    val meanX = Xvalues.data.reduce((x,y) => x + y) / Xvalues.data.length
    val diffX = Xvalues.data.map(x => x - meanX)
    val sumOfSquaredDiffX = diffX.map(x => x * x).reduce((x,y) => x + y)
    
    // square root of (Sum of squared residuals divided by degrees of Freedom) divided by (sum of Squared Diff X))
    val standardError = 
      math.sqrt( (sumOfSquaredResiduals / degreesOfFreedom) / sumOfSquaredDiffX )
    
    // Test statistic: T = slope / standardError
    val T = slope / standardError
    
   /* Calculate p from studentT distribution
    * 	The cumulative probability is the probability that the true T is less than the absolute value
    *   of the t we calculated
    * 	So 1 - cumulativeProb(T) is the probability that it is greater than our t
    *   Because this is a two-tailed test, we multiply this result by 2 to get our final p-value
    *   Note: We use the absolute value of T here to produce the correct value,
    *     because the t-distribution is symmetric
    */
    
    val p_value = (1 - new TDistribution(degreesOfFreedom).cumulativeProbability(abs(T))) * 2

    // Return regression output
    return RegressionOutput(SNP_name = SNP._1,
                            phenotype_name = Pheno._1,
                            coefficient = slope,
                            intercept = intercept,
                            p_value = p_value,
                            SNP_values = SNP._2
                    )    
  }
  
  def conductRegressions(
      SNPs: rdd.RDD[(String, Vector[Double])],
      phenotype: (String, Vector[Double])
      ): RegressionOutput = {
    
    // Returns the regression information with the smallest p-value from a given
    //   collection of SNPs and a phenotype
    
    val outputs = SNPs.mapPartitions(
        // On the collection found within each partition
        partition => partition.map(
            // Perform SLR on each element of the collection (against the phenotype)
            snp => performSimpleLinearRegression(snp, phenotype)) , preservesPartitioning = true)
    // Outputs is an RDD of the results from each regression (still partitioned out over the cluster)
    outputs.reduce( (x, y) => if (x.p_value < y.p_value) x else y )
  }
  
  @tailrec
  def ConductForwardSteps(
      session: SparkSession,
      phenotype_vector: (String, Vector[Double]),
      SNPs: rdd.RDD[(String, Vector[Double])],
      model: Vector[ModelEntry] = Vector()
      ): Vector[ModelEntry] = {
    
    // Broadcast the phenotype_vector
    val broadcast_phenotype = session.sparkContext.broadcast(phenotype_vector)
    
    // Perform the regressions and find the minimum p-value
    val best_SNP = conductRegressions(SNPs, broadcast_phenotype.value)
    
    // Remove old broadcast phenotype
    broadcast_phenotype.destroy()
    
    if (best_SNP.p_value <= threshold) {  
      
      // Create new phenotype vector
      // The new Y will be Y - slope1*X1 (where the slope1 and X1 are what was just added to the model)  
      val new_Y_values = for (i <- 0 until best_SNP.SNP_values.length) yield {
          // Yi - (Xi * coefficient)
          phenotype_vector._2(i) - ( best_SNP.SNP_values(i) * best_SNP.coefficient )
      }
      
      val new_Y_name = phenotype_vector._1 + "-" + best_SNP.SNP_name

      val new_phenotype = (new_Y_name, new_Y_values.toVector)
      
      // Remove the SNP from the SNP RDD that is passed to the next iteration (the original pairwise RDD is
      // persisted across the cluster.)
      // Keep if SNP is not equal to the best one found in this iteration
      val updated_SNPs = SNPs.filter(x => x._1 != best_SNP.SNP_name)
      
      val newModelEntry = new ModelEntry(SNP_name = best_SNP.SNP_name,
                                         coefficient = best_SNP.coefficient,
                                         p_value = best_SNP.p_value
                                        )
      
      val updatedModel = model :+ newModelEntry
      
      if (updated_SNPs.isEmpty()) {
        // There are no more SNPs to be tested
        // Return the updated model
        return updatedModel
      
      } else {
        // Call this function again with updated info
        ConductForwardSteps(session, new_phenotype, updated_SNPs, updatedModel)
      }
    } else {
      /*
       * 	There are no more SNPs that meet our threshold
       *  Forward Step has been completed
       *  Return the final forward model
       */
      return model
    }
  }
  
  def main(args: Array[String]) {
    
    // 1st argument: SNP file
    // 2nd argument: Phenotype file
    val SNP_file = args(0)
    val pheno_file = args(1)
        
    /*
     *  Define spark session
     */
    val spark = SparkSession.builder.master("local").appName("Epistasis").getOrCreate()
        
    /*
     *  Parse the input files
     */
    val SNPdata = parseInputFile(SNP_file)
    val phenoData = parseInputFile(pheno_file)

    // Create list of (non-redundant) pairwise combinations and partition the list across the cluster
    val pairwiseList = spark.sparkContext.parallelize(createPairwiseList(SNPdata))  
    
    /*
     * Broadcast the SNPdata to each node (as a map so that values can be retrieved by SNP name)
     * This scala-based Key-value association is only used to create the pairwise combinations
     */
    val broadSNP: Broadcast[Map[String,Vector[Double]]] = spark.sparkContext.broadcast(SNPdata.toMap)
    
    /* Original SNPs
     * This is the same data used in the scala key-value association defined in the broadSNP variable,
     *   only now these are distributed across the cluster, i.e., it is a Spark key-value association, not
     *   the simpler scala map
     */
    val originalSNPs: rdd.RDD[(String, Vector[Double])] = spark.sparkContext.parallelize(SNPdata)
    
    // Multiply the values for each pairwise combination to create a new column
    val pairwiseSNPs: rdd.RDD[(String, Vector[Double])] = 
      pairwiseList.map(namePairs => createPairwiseCombinations(namePairs, broadSNP))
      
    // Combine the original and pairwise RDDs into one RDD
    val allSNPs = spark.sparkContext.union(originalSNPs, pairwiseSNPs)

    val model = ConductForwardSteps(spark, phenoData(0), allSNPs)
    
    model.foreach(println)
   
  }
  
}
