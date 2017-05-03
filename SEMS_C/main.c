#include <stdio.h>
#include "args.h"
#include "data.h"
#include "ols_alg.h"

#define NUM_TEST 100
#define PHEN_NUM 0

int main(int argc, char** argv){

    /* Initialize structs. */
    args* my_args = NULL;
    genotype* my_genotype = NULL;
    phenotype* my_phenotype = NULL;
    
    float z[NUM_TEST][2];   /* Result array.   */
    int i;                  /* Loop variables. */
    
    
    
    /* Load parameters. */
    if((my_args = get_params(argc, argv)) == NULL)
        return 1;
    
    
    
    /* Print file names. */
    /*
    printf("\nGenotype File: %s\nPhenotype File: %s\nOutput File: %s\n",
           my_args->genotypeFile, my_args->phenotypeFile, my_args->outputFile);
    */
    
    
    
    /* Load genotype. */
    if((my_genotype = genotype_load(my_args->genotypeFile)) == NULL){
        fprintf(stderr, "NULL: my_genotype\n");
        return 1;
    }
    
    /* Print genotype. */
    /*
    printf("\nGenotype File: %s\nNumber of Individuals: %d\nNumber of Markers: %d\n",
           my_args->genotypeFile, my_genotype->n_individual, my_genotype->n_marker);
    
    printf("\nGenotype Data:\n");
    
    for(i = 0; i < my_genotype->n_marker; i++)
        printf("%s ", my_genotype->marker[i]);
    printf("\n");
    for(i = 0; i < my_genotype->n_individual; i++){
        printf("%s ", my_genotype->individual[i]);
        for(j = 0; j < my_genotype->n_marker; j++)
            printf("%f ", my_genotype->matrix[j][i]);
        printf("\n");
    }
    printf("\n");
    */
    
    
    
    /* Load phenotype. */
    if((my_phenotype = phenotype_load(my_args->phenotypeFile)) == NULL){
        fprintf(stderr, "NULL: my_phenotype\n");
        return 1;
    }
    
    /* Print phenotype. */
    /*
    printf("\nPhenotype File: %s\nNumber of Individuals: %d\nNumber of Traits: %d\n",
           my_args->phenotypeFile, my_phenotype->n_individual, my_phenotype->n_trait);
    
    printf("\nPhenotype Data:\n");
    
    for(i = 0; i < my_phenotype->n_trait; i++)
        printf("%s ", my_phenotype->trait[i]);
    printf("\n");
    for(i = 0; i < my_phenotype->n_individual; i++){
        printf("%s ", my_phenotype->individual[i]);
        for(j = 0; j < my_phenotype->n_trait; j++)
            printf("%f ", my_phenotype->matrix[j][i]);
        printf("\n");
    }
    printf("\n");
    */

    
    
    /* Print ols analysis. */
    
    printf("\nLeast Squares Regression Analysis:\n\n");
    
    for(i = 0; i < NUM_TEST; i++){
        printf("Marker %d, Trait %d\n", i, PHEN_NUM);
        
        ols_regression_analysis(my_genotype->matrix[i], my_phenotype->matrix[PHEN_NUM], z[i], 
                                my_genotype->n_individual, 1, 1);

        printf("a: %f\nb: %f\n\n", z[i][1], z[i][0]);
    }
    
    
    
    /* Free structs. */
    free_genotype(my_genotype);
    free_phenotype(my_phenotype);
    free_params(my_args);
    
    return 0;
}

