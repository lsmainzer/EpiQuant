/* Data : Function Definition File */

#include "data.h"

/*
 * genotype_load
 *   DESCRIPTION: Creates and fills a genotype struct by 
 *                passing in a text file of a specific format.
 *   INPUTS: fileName -- name of text file to be read.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated genotype struct.
 *   SIDE EFFECTS: Allocates a genotype struct.
 */
genotype* genotype_load(char* fileName){

    FILE* f = NULL;             /* File variable.         */
    genotype* g_t = NULL;       /* Return argument.       */
    char s[32];                 /* Temp variables.        */
    float d = 0.0;          
    int i = 0;                  /* Loop variables.        */
    int j = 0;
    
    int n_marker = -2;          /* Initial marker offset. */
    int n_individual = 1;            /* Initial individual offset.  */
    
    
    
    /* Open file error handling. */
    if((f = fopen(fileName, "r")) == NULL){
        fprintf(stderr, "file \"%s\" does not exist\n", fileName);
        return NULL;
    }
    
    
    
    /* Intial counting to determine n_marker nad n_individual. */
    /* Skip first two entries. */
    fscanf(f, "%*s %*s");
    
    do{
        n_marker++;
        fscanf(f, "%s", s);
    }while(!isdigit(s[0]));
    
    while(fscanf(f, "%s", s) != EOF){
        if(!isdigit(s[0]))
            n_individual++;
    }
    
    fprintf(stderr, "Markers: %d\nIndividuals: %d\n", n_marker, n_individual);
    
    /* Struct memory allocation. */
    if((g_t = (genotype*)malloc(sizeof(genotype))) == NULL){
        fprintf(stderr, "cannot allocate memory: genotype*\n");
        fclose(f);
        free_genotype(g_t);
        return NULL;
    }
    
    
    
    /* Marker memory allocation. */
    if((g_t->marker = (char**)malloc(n_marker * sizeof(char*))) == NULL){
        fprintf(stderr, "cannot allocate memory: g.marker**\n");
        fclose(f);
        free_genotype(g_t);
        return NULL;
    }
    for(i = 0; i < n_marker; i++){
        if((g_t->marker[i] = (char*)malloc(sizeof(char) * 32)) == NULL){
            fprintf(stderr, "cannot allocate memory: g.marker*\n");
            fclose(f);
            free_genotype(g_t);
            return NULL;
        }
    }
    
    
    
    /* individual memory allocation. */
    if((g_t->individual = (char**)malloc(n_individual * sizeof(char*))) == NULL){
        fprintf(stderr, "cannot allocate memory: g.individual**\n");
        fclose(f);
        free_genotype(g_t);
        return NULL;
    }
    for(i = 0; i < n_individual; i++){
        if((g_t->individual[i] = (char*)malloc(sizeof(char) * 32)) == NULL){
            fprintf(stderr, "cannot allocate memory: g.individual*\n");
            fclose(f);
            free_genotype(g_t);
            return NULL;
        }
    }
    
    
    
    /* 2-D matrix memory allocation. */
    if((g_t->matrix = (float**)malloc(n_marker * sizeof(float*))) == NULL){
        fprintf(stderr, "cannot allocate memory: g.matrix**\n");
        fclose(f);
        free_genotype(g_t);
        return NULL;
    }
    for(i = 0; i < n_marker; i++){
        if((g_t->matrix[i] = (float*)malloc(n_individual * sizeof(float))) == NULL){
            fprintf(stderr, "cannot allocate memory: g.matrix*\n");
            fclose(f);
            free_genotype(g_t);
            return NULL;
        }
    }
    
    
    
    /* Rewind file. */
    if(fseek(f, 0, SEEK_SET) != 0)
        fprintf(stderr, "could not rewind file");
    
    /* Skip first two entries. */
    fscanf(f, "%*s %*s");
    
    /* Marker assignment. */
    for(i = 0; i < n_marker; i++){
        fscanf(f, "%s", s);
        strcpy(g_t->marker[i], s);
    }
    
    /* individual and matrix assignment (column major order). */
    for(i = 0; i < n_individual; i++){
        fscanf(f, "%s", s);
        strcpy(g_t->individual[i], s);
        
        for(j = 0; j < n_marker; j++){
            fscanf(f, "%f", &d);
            g_t->matrix[j][i] = d;
        }
    }
    
    /* Other assignments. */
    g_t->n_marker = n_marker;
    g_t->n_individual = n_individual;
    
    /* Close file. */
    fclose(f);
    
    /* Return. */
    return g_t;
}

/*
 * genotype_store
 *   DESCRIPTION: Stores the genotype struct data into a
 *                text file in a specific format.
 *   INPUTS: fileName -- name of text file to be written to.
 *           g_t -- pointer to genotype struct.
 *   OUTPUTS: Writes to fileName.
 *   RETURN VALUE: (0) on success, (1) on failure.
 *   SIDE EFFECTS: None.
 */
/*
int genotype_store(char* fileName, genotype* g_t){
    return 1;
}
*/

/*
 * free_genotype
 *   DESCRIPTION: Deallocates memory associated with a genotype struct.
 *   INPUTS: g_t -- pointer to genotype struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates a genotype struct.
 */
void free_genotype(genotype* g_t){
    int i = 0;                  /* Loop variable. */
    
    if(g_t != NULL){
        /* Outer free. */
        if(g_t->marker != NULL){
            /* Inner free. */
            for(i = 0; i < g_t->n_marker; i++){
                free(g_t->marker[i]);
                g_t->marker[i] = NULL;
            }
        }
        if(g_t->individual != NULL){
            for(i = 0; i < g_t->n_individual; i++){
                free(g_t->individual[i]);
                g_t->individual[i] = NULL;
            }
        }
        if(g_t->matrix != NULL){
            for(i = 0; i < g_t->n_marker; i++){
                free(g_t->matrix[i]);
                g_t->matrix[i] = NULL;
            }
        }
        free(g_t);
        g_t = NULL;
    }
}



/*
 * phenotype_load
 *   DESCRIPTION: Creates and fills a phenotype struct by 
 *                passing in a text file of a specific format.
 *   INPUTS: fileName -- name of text file to be read.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated phenotype struct.
 *   SIDE EFFECTS: Allocates a phenotype struct.
 */
phenotype* phenotype_load(char* fileName){

    FILE* f = NULL;             /* File variable.         */
    phenotype* p_t = NULL;      /* Return argument.       */
    char s[32];                 /* Temp variables.        */
    float d = 0.0;
    int i = 0;                  /* Loop variables.        */
    int j = 0;
    
    int n_trait = -2;           /* Initial trait offset.  */
    int n_individual = 1;            /* Initial individual offset.  */
    
    
    
    /* Open file error handling. */
    if((f = fopen(fileName, "r")) == NULL){
        fprintf(stderr, "file \"%s\" does not exist\n", fileName);
        return NULL;
    }
    
    
    
    /* Intial counting to determine n_trait nad n_individual. */
    /* Skip first entry. */
    fscanf(f, "%*s");
    
    do{
        n_trait++;
        fscanf(f, "%s", s);
    }while((!isdigit(s[0])) && (s[0] != '-'));
    
    while(fscanf(f, "%s", s) != EOF){
        if((!isdigit(s[0])) && (s[0] != '-'))
            n_individual++;
    }
    
    fprintf(stderr, "Traits: %d\nIndividuals: %d\n", n_trait, n_individual);
    
    /* Struct memory allocation. */
    if((p_t = (phenotype*)malloc(sizeof(phenotype))) == NULL){
        fprintf(stderr, "cannot allocate memory: phenotype*\n");
        fclose(f);
        free_phenotype(p_t);
        return NULL;
    }
    
    
    
    /* trait memory allocation. */
    if((p_t->trait = (char**)malloc(n_trait * sizeof(char*))) == NULL){
        fprintf(stderr, "cannot allocate memory: p.trait**\n");
        fclose(f);
        free_phenotype(p_t);
        return NULL;
    }
    for(i = 0; i < n_trait; i++){
        if((p_t->trait[i] = (char*)malloc(sizeof(char) * 32)) == NULL){
            fprintf(stderr, "cannot allocate memory: p.trait*\n");
            fclose(f);
            free_phenotype(p_t);
            return NULL;
        }
    }
    
    
    /* individual memory allocation. */
    if((p_t->individual = (char**)malloc(n_individual * sizeof(char*))) == NULL){
        fprintf(stderr, "cannot allocate memory: p.individual**\n");
        fclose(f);
        free_phenotype(p_t);
        return NULL;
    }
    for(i = 0; i < n_individual; i++){
        if((p_t->individual[i] = (char*)malloc(sizeof(char) * 32)) == NULL){
            fprintf(stderr, "cannot allocate memory: p.individual*\n");
            fclose(f);
            free_phenotype(p_t);
            return NULL;
        }
    }
    
    
    /* 2-D matrix memory allocation. */
    if((p_t->matrix = (float**)malloc(n_trait * sizeof(float*))) == NULL){
        fprintf(stderr, "cannot allocate memory: p.matrix**\n");
        fclose(f);
        free_phenotype(p_t);
        return NULL;
    }
    for(i = 0; i < n_trait; i++){
        if((p_t->matrix[i] = (float*)malloc(n_individual * sizeof(float))) == NULL){
            fprintf(stderr, "cannot allocate memory: p.matrix*\n");
            fclose(f);
            free_phenotype(p_t);
            return NULL;
        }
    }
    
    
    /* Rewind file. */
    if(fseek(f, 0, SEEK_SET) != 0)
        fprintf(stderr, "could not rewind file");
    /* Skip first entry. */
    fscanf(f, "%*s");
    
    
    
    /* Trait assignment. */
    for(i = 0; i < n_trait; i++){
        fscanf(f, "%s", s);
        strcpy(p_t->trait[i], s);
    }
    
    /* individual and matrix assignment (column major order). */
    for(i = 0; i < n_individual; i++){
        fscanf(f, "%s", s);
        strcpy(p_t->individual[i], s);
        
        for(j = 0; j < n_trait; j++){
            fscanf(f, "%f", &d);
            p_t->matrix[j][i] = d;
        }
    }
    
    /* Other assignments. */
    p_t->n_trait = n_trait;
    p_t->n_individual = n_individual;
    
    /* Close file. */
    fclose(f);
    
    /* Return. */
    return p_t;
}

/*
 * phenotype_store
 *   DESCRIPTION: Stores the phenotype struct data into a
 *                text file in a specific format.
 *   INPUTS: fileName -- name of text file to be written to.
 *           p_t -- pointer to phenotype struct.
 *   OUTPUTS: Writes to fileName.
 *   RETURN VALUE: (0) on success, (1) on failure.
 *   SIDE EFFECTS: None.
 */
/*
int phenotype_store(char* fileName, phenotype* p_t){
    return 1;
}
*/

/*
 * free_phenotype
 *   DESCRIPTION: Deallocates memory associated with a phenotype struct.
 *   INPUTS: p_t -- pointer to phenotype struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates a phenotype struct.
 */
void free_phenotype(phenotype* p_t){
    int i = 0;                  /* Loop variable. */
    
    if(p_t != NULL){
        /* Outer free. */
        if(p_t->trait != NULL){
            /* Inner free. */
            for(i = 0; i < p_t->n_trait; i++){
                free(p_t->trait[i]);
                p_t->trait[i] = NULL;
            }
        }
        if(p_t->individual != NULL){
            for(i = 0; i < p_t->n_individual; i++){
                free(p_t->individual[i]);
                p_t->individual[i] = NULL;
            }
        }
        if(p_t->matrix != NULL){
            for(i = 0; i < p_t->n_trait; i++){
                free(p_t->matrix[i]);
                p_t->matrix[i] = NULL;
            }
        }
        free(p_t);
        p_t = NULL;
    }
}

