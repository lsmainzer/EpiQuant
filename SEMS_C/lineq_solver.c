/* Linear Equation Solver : Function Definition File */

#include "lineq_solver.h"

/* Layout of matrices. */
#define LP_LAYOUT   LAPACK_COL_MAJOR

/* Parameters */
/*
#define N    1
#define NRHS 1
#define LDA  N
#define LDB  N
*/

/*
 * lineq_solver
 *   DESCRIPTION: Performs the linear euqation solver on two matrices in the
 *                form AX = B
 *   INPUTS: A -- A matrix in equation.
 *           X -- X matrix in equation.
 *           B -- B matrix in euqation.
 *           n_individuals -- number of individuals : rows in A, rows in B.
 *           n_markers     -- number of markers     : cols in A.
 *           n_traits      -- number of traits      : cols in B.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Writes to A, X, B.
 *
 *   int n_individuals, int n_markers, int n_traits
 *   float* x
 */
void lineq_solver(float* a, float* b, int n_individuals, int n_markers, int n_traits){
    
    /* Locals */
    int i;
    int size_a = n_individuals * n_individuals;
    int size_b = n_individuals * n_traits;
    lapack_int ret_val;
    
    /* Local arrays */
    float* _a = NULL;
    float* _b = NULL;
    lapack_int ipiv[n_individuals];
    
    /* Allocate memory for _a, a n-by-n copy of a. */
    if((_a = (float*)malloc(size_a * sizeof(float))) == NULL){
        fprintf(stderr, "cannot allocate memory: _a**\n");
        return;
    }
    
    /* Allocate memory for _b, a copy of b. */
    if((_b = (float*)malloc(size_b * sizeof(float))) == NULL){
        fprintf(stderr, "cannot allocate memory: _b**\n");
        return;
    }
    
    /* Fill arrays. */
    for(i = 0; i < size_a; i++){
        if(i >= n_individuals * (n_markers + 1))
            _a[i] = 0.0;
        else if(i >= n_individuals * n_markers)
            _a[i] = 1.0;
        else
            _a[i] = a[i];
    }
    
    for(i = 0; i < size_b; i++)
        _b[i] = b[i];
    
    /* Solve the equations A*X = B. */
    if((ret_val = LAPACKE_sgesv(LP_LAYOUT, n_individuals, n_traits, _a,
                                n_individuals, ipiv, _b, n_individuals)) != 0){
        fprintf(stderr, "Linear Equation Solver unsuccessful: info = %lli\n", ret_val);
        free(_a);
        free(_b);
        return;
    }
    
    /* Check for the exact singularity. */
    /*
    if( info > 0 ) {
            fprintf(stderr, "The diagonal element of the triangular factor of A,\n" );
            fprintf(stderr, "U(%i,%i) is zero, so that A is singular;\n", info, info );
            fprintf(stderr, "the solution could not be computed.\n" );
            free(_a);
            free(_b);
            return;
    }
    */
    
    /* Print solution */
    print_matrix("Solution:", _b, 1, n_markers + 1);
    
    /* Print details of LU factorization */
    /*
    print_matrix( "Details of LU factorization", n, n, a, lda );
    */
    
    free(_a);
    free(_b);
    return;
}

/* Auxiliary routine: printing a matrix */
void print_matrix(char* desc, float* matrix, int rows, int cols){
    int i;
    printf("\n%s\n", desc);
    for(i = 0; i < rows * cols; i++)
        printf("%f\n", matrix[i]);
    printf("\n");
}

