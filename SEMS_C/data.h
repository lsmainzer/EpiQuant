/* Data : Header File */

#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int n_marker;
    int n_individual;
    
    char** individual;
    char** marker;
    
    float** matrix;
} genotype;



typedef struct {
    int n_trait;
    int n_individual;
    
    char** individual;
    char** trait;
    
    float** matrix;
} phenotype;



/*
 * genotype_load
 *   DESCRIPTION: Creates and fills a genotype struct by 
 *                passing in a text file of a specific format.
 *   INPUTS: fileName -- name of text file to be read.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated genotype struct.
 *   SIDE EFFECTS: Allocates a genotype struct.
 */
genotype* genotype_load(char* fileName);

/*
 * genotype_store
 *   DESCRIPTION: Stores the genotype struct data into a
 *                text file in a specific format.
 *   INPUTS: fileName -- name of text file to be written to.
 *           g_t -- pointer to genotype struct.
 *   OUTPUTS: fileName
 *   RETURN VALUE: (0) on success, (1) on failure.
 *   SIDE EFFECTS: Writes to fileName
 */
int genotype_store(char* fileName, genotype* g_t);

/*
 * free_genotype
 *   DESCRIPTION: Deallocates memory associated with a genotype struct.
 *   INPUTS: g_t -- pointer to genotype struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates a genotype struct.
 */
void free_genotype(genotype* g_t);



/*
 * phenotype_load
 *   DESCRIPTION: Creates and fills a phenotype struct by 
 *                passing in a text file of a specific format.
 *   INPUTS: fileName -- name of text file to be read.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated phenotype struct.
 *   SIDE EFFECTS: Allocates a phenotype struct.
 */
phenotype* phenotype_load(char* fileName);

/*
 * phenotype_store
 *   DESCRIPTION: Stores the phenotype struct data into a
 *                text file in a specific format.
 *   INPUTS: fileName -- name of text file to be written to.
 *           p_t -- pointer to phenotype struct.
 *   OUTPUTS: fileName
 *   RETURN VALUE: (0) on success, (1) on failure.
 *   SIDE EFFECTS: Writes to fileName
 */
int phenotype_store(char* fileName, phenotype* p_t);

/*
 * free_phenotype
 *   DESCRIPTION: Deallocates memory associated with a phenotype struct.
 *   INPUTS: p_t -- pointer to phenotype struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates a phenotype struct.
 */
void free_phenotype(phenotype* p_t);

#endif
