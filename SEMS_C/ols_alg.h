/* Ordinary Least Squares Algorithms : Header File */

/* 
 * In order to include the necessary libraries, download Intel MKL at:
 * https://software.intel.com/en-us/intel-mkl
 * Next, download Intel MPI Library at:
 * https://software.intel.com/en-us/intel-mpi-library
 *
 * Afterwards, use the command:
 *     % source /opt/intel/compilers_and_libraries_2017.2.174/linux/mkl/bin/mklvars.sh intel64
 *     % source /opt/intel/compilers_and_libraries_2017.2.174/linux/mpi/bin64/mpivars.sh
 * before executing the program to correctly link the file.
 *
 * You may need to change the file path accordingly.
 */

#ifndef OLS_ALG_H
#define OLS_ALG_H

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"
#include "mkl_scalapack.h"

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
                             int n_individuals, int n_markers, int n_traits);

/*
 * inverseMat
 *   DESCRIPTION: Performs the inverse matrix computation on X.
 *   INPUTS: x -- X matrix.
 *           n -- number of rows in X.
 *   OUTPUTS: None.
 *   RETURN VALUE: info integer for error debugging.
 *   SIDE EFFECTS: Writes to x.
 */
lapack_int inverseMat(float *x, int n);

#endif
