/* Linear Equation Solver : Header File */

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

#ifndef LINEQ_SOLVER_H
#define LINEQ_SOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"
#include "mkl_scalapack.h"

/*
 * lineq_solver
 *   DESCRIPTION: Performs the linear euqation solver on two matrices in the
 *                form AX = B
 *   INPUTS: A -- A matrix in equation.
 *           X -- X matrix in equation.
 *           B -- B matrix in euqation.
 *           n_individuals -- number of individuals : rows in A and B.
 *           n_markers     -- number of markers     : cols in X, rows in Z.
 *           n_traits      -- number of traits      : rows in Y, cols in Z.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Writes to X.
 *
 *   int n_individuals, int n_markers, int n_traits
 *   float* x
 */
void lineq_solver(float* a, float* b, int n_individuals, int n_markers, int n_traits);

void print_matrix(char* desc, float* matrix, int rows, int cols);

#endif
