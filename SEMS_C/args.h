/* Argument : Header File */

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

typedef struct {
    char* genotypeFile;
    char* phenotypeFile;
    char* outputFile;
    
    int n_individual;
    int n_marker;
    int n_trait;
} args;



/*
 * get_params
 *   DESCRIPTION: Creates and fills an args struct by parsing commaind line arguments.
 *   INPUTS: argc -- argument count.
 *           argv -- argument vector.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated args struct.
 *   SIDE EFFECTS: Allocates an args struct.
 */
args* get_params(int argc, char** argv);

/*
 * free_params
 *   DESCRIPTION: Deallocates memory associated with an args struct.
 *   INPUTS: a -- pointer to args struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates an args struct.
 */
void free_params(args* a);

#endif
