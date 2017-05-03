/* Ordinary Least Squares Algorithms : Function Definition File */

#include "ols_alg.h"

/* Layout of matrices. */
#define LAYOUT      CblasColMajor
#define LP_LAYOUT   LAPACK_COL_MAJOR

/*
 * ols_regression_analysis
 *   DESCRIPTION: Performs the ordinary least squares regression analysis
 *                on two matrices in the form (X^T * X)^(-1) * (X^T * Y).
 *   INPUTS: x -- X matrix in equation
 *           y -- Y matrix in equation
 *           z -- output matrix
 *           n_individuals -- number of individuals : rows in X and Y.
 *           n_markers     -- number of markers     : cols in X, rows in Z.
 *           n_traits      -- number of traits      : rows in Y, cols in Z.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Writes to z.
 */
void ols_regression_analysis(float* x, float* y, float* z, 
                             int n_individuals, int n_markers, int n_traits){

    float* _x = NULL;                   /* Matrix with added column of 1's. */
    float* matrix1 = NULL;              /* (X^T * X)^(-1)                   */
    float* matrix2 = NULL;              /* (X^T * Y)                        */

    int size_x = (n_individuals * n_markers) + n_individuals; /* Size of allocated arrays. */
    int size_matrix1 = (n_markers + 1) * (n_markers + 1);
    int size_matrix2 = (n_markers + 1) * n_traits;
    
    int i, j;                           /* Loop variables.                  */

    /* Allocate memory for _x, x with a column of 1's. */
    if((_x = (float*)malloc(size_x * sizeof(float))) == NULL){
        fprintf(stderr, "cannot allocate memory: _x**\n");
        return;
    }

    /* Allocate memory for first product. */
    if((matrix1 = (float*)malloc(size_matrix1 * sizeof(float))) == NULL){
        fprintf(stderr, "cannot allocate memory: matrix1**\n");
        return;
    }

    /* Allocate memory for second product. */
    if((matrix2 = (float*)malloc(size_matrix2 * sizeof(float))) == NULL){
        fprintf(stderr, "cannot allocate memory: matrix2**\n");
        return;
    }

    /* Add column of 1's to x */
    for(i = 0; i < n_individuals; i++){
        _x[i] = 1.0;
        for(j = 0; j < n_markers; j++)
            _x[n_individuals + (n_individuals * j) + i] = x[(n_individuals * j) + i];
    }
    
    /*
    fprintf(stderr, "_x:\n");
    for(i = 0; i < n_individuals; i++)
        fprintf(stderr, "%f %f\n", _x[i], _x[n_individuals + i]);
    fprintf(stderr, "\n");
    */
    
    /* (X^T * X) */
    cblas_sgemm(LAYOUT, CblasTrans, CblasNoTrans,
                n_markers + 1, n_markers + 1, n_individuals,
                1,
                _x, n_individuals,
                _x, n_individuals,
                0,
                matrix1, n_markers + 1);
    
    /*
    fprintf(stderr, "(X^T * X):\n");
    for(i = 0; i < n_markers + 1; i++)
        fprintf(stderr, "%f %f\n", matrix1[i], matrix1[i + n_markers + 1]);
    fprintf(stderr, "\n");
    */
    
    /* (X^T * X)^(-1) */
    inverseMat(matrix1, n_markers + 1);
    
    /*
    fprintf(stderr, "(X^T * X)^(-1):\n");
    for(i = 0; i < n_markers + 1; i++)
        fprintf(stderr, "%f %f\n", matrix1[i], matrix1[i + n_markers + 1]);
    fprintf(stderr, "\n");
    */
    
    /* (X^T * Y) */
    cblas_sgemm(LAYOUT, CblasTrans, CblasNoTrans,
                n_markers + 1, n_traits, n_individuals,
                1,
                _x, n_individuals,
                y, n_individuals,
                0,
                matrix2, n_markers + 1);
    
    /*
    fprintf(stderr, "(X^T * Y):\n");
    for(i = 0; i < n_markers + 1; i++)
        fprintf(stderr, "%f\n", matrix2[i]);
    fprintf(stderr, "\n");
    */
    
    /* (X^T * X)^(-1) * (X^T * Y) */
    cblas_sgemm(LAYOUT, CblasNoTrans, CblasNoTrans,
                n_markers + 1, n_traits, n_markers + 1,
                1,
                matrix1, n_markers + 1,
                matrix2, n_markers + 1,
                0,
                z, n_markers + 1);
    
    /*
    fprintf(stderr, "(X^T * X)^(-1) * (X^T * Y):\n");
    for(i = 0; i < n_markers + 1; i++)
        fprintf(stderr, "%f\n", z[i]);
    fprintf(stderr, "\n");
    */
    
    /* Free allocated matrices. */
    free(_x);
    free(matrix1);
    free(matrix2);
    
    return;
}


/*
 * invMat
 *   DESCRIPTION: Performs the inverse matrix computation on X.
 *   INPUTS: x -- X matrix.
 *           n -- number of rows in X.
 *   OUTPUTS: None.
 *   RETURN VALUE: info integer for error debugging.
 *   SIDE EFFECTS: Writes to x.
 */
lapack_int inverseMat(float *x, int n){

    lapack_int info;            /* Return value. */
    lapack_int ipiv[n + 1];     /* Pivot array.  */

    /* Computes the LU factorization of a general n-by-n matrix. */
    if(0 != (info = LAPACKE_sgetrf(LP_LAYOUT,
                                   n, n, 
                                   x,
                                   n,
                                   ipiv))){
        fprintf(stderr, "sgetrf: info = %lld", info);
        return info;
    }

    /* Computes the inverse of an LU-factored general matrix. */
    if(0 != (info = LAPACKE_sgetri(LP_LAYOUT,
                                   n,
                                   x,
                                   n,
                                   ipiv))){
        fprintf(stderr, "sgetrf: info = %lld", info);
        return info;
    }
    
    return info;
}



