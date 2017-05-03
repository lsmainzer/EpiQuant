/* Argument : Function Definition File */

#include "args.h"

/*
 * get_params
 *   DESCRIPTION: Creates and fills an args struct by parsing commaind line arguments.
 *   INPUTS: argc -- argument count.
 *           argv -- argument vector.
 *   OUTPUTS: None.
 *   RETURN VALUE: Pointer to newly allocated args struct.
 *   SIDE EFFECTS: Allocates an args struct.
 */
args* get_params(int argc, char** argv){
    
    int i = 0;                      /* Loop variable.    */
    int opt = 0;                    /* Option variables. */
    int option_index = 0;
    
    /* Flags. */
    int errflag = 0;
    int gflag = 0;
    int pflag = 0;
    int oflag = 0;
    int nflag = 0;
    int mflag = 0;
    int rflag = 0;
    int hflag = 0;
    
    /* Option Arguments. */
    char* g_opt_arg = NULL;
    char* p_opt_arg = NULL;
    char* o_opt_arg = NULL;
    int n_opt_arg = -1;
    int m_opt_arg = -1;
    int r_opt_arg = -1;
    
    /* Return argument. */
    args* my_args = NULL;
    
    /* Allocate memory. */
    if((my_args = (args*)malloc(sizeof(args))) == NULL){
        fprintf(stderr, "cannot allocate memory\n");
        return NULL;
    }
    
    /* Define long_option struct. */
    static struct option long_options[] = {
        {"help",      no_argument,       NULL, 'h'},
        {"genotype",  required_argument, NULL, 'g'},
        {"phenotype", required_argument, NULL, 'p'},
        {"output",    required_argument, NULL, 'o'},
        {"individual",optional_argument, NULL, 'n'},
        {"marker",    optional_argument, NULL, 'm'},
        {"trait",     optional_argument, NULL, 'r'},
        {0, 0, 0, 0}
    };
    
    /* Start getopt_long_only option parsing loop. */
    while((opt = getopt_long_only(argc, argv, "hg:p:o:nmr", long_options, &option_index)) != -1){
        switch(opt){
            case 'g':
                g_opt_arg = optarg;
                gflag++;
                break;
            case 'p':
                p_opt_arg = optarg;
                pflag++;
                break;
            case 'o':
                o_opt_arg = optarg;
                oflag++;
                break;
            case 'n':
                n_opt_arg = atoi(optarg);
                nflag++;
                break;
            case 'm':
                m_opt_arg = atoi(optarg);
                mflag++;
                break;
            case 'r':
                r_opt_arg = atoi(optarg);
                rflag++;
                break;
            
            /* Help. */
            case 'h':
                printf("\nMAIN ARGUMENTS:\n");
                printf("\nRequired Arguments:\n");
                printf("    -genotype   (-g)  |  input:  genotype file         |  example: -g file.txt\n");
                printf("    -phenotype  (-p)  |  input:  phenotype file        |  example: -p file.txt\n");
                printf("    -output     (-o)  |  output: output file           |  example: -o file.txt\n");
                printf("\nOptional Arguments:\n");
                printf("    -individual (-n)  |  input: number of individuals  |  example: -t 1000\n");
                printf("    -marker     (-m)  |  input: number of markers      |  example: -m 1000\n");
                printf("    -trait      (-r)  |  input: number of traits       |  example: -r 1000\n\n");
                hflag++;
                errflag++;
                break;
                
            /* Error handling. */
            case '?':
                /* Default getopt error message. */
                errflag++;
                break;
            case 0:
                fprintf(stderr, "%s: invalid flag pointer\n", argv[0]);
                errflag++;
                break;
            default:
                fprintf(stderr, "%s: illegal arguments\n", argv[0]);
                errflag++;
                break;
        }
        
        if(errflag){
            if(!hflag)
                printf("use \"-help\" for a description of valid arguments\n");
            return NULL;
        }
    }
    
    /* More error handling. */
    for(i = optind; i < argc; i++)
        fprintf(stderr, "non-option argument: %s\n", argv[i]);
    
    if(!gflag || !pflag || !oflag){
        fprintf(stderr, "not all required arguments satisfied\n");
        printf("use \"-help\" for a description of valid arguments\n");
        return NULL;
    }
    
    /* Fill args struct. */
    my_args->genotypeFile = g_opt_arg;
    my_args->phenotypeFile = p_opt_arg;
    my_args->outputFile = o_opt_arg;
    my_args->n_individual = n_opt_arg;
    my_args->n_marker = m_opt_arg;
    my_args->n_trait = r_opt_arg;
    
    return my_args;
}

/*
 * free_params
 *   DESCRIPTION: Deallocates memory associated with an args struct.
 *   INPUTS: my_args -- pointer to struct.
 *   OUTPUTS: None.
 *   RETURN VALUE: None.
 *   SIDE EFFECTS: Deallocates an args struct.
 */
void free_params(args* a){
    if(a != NULL)
        free(a);
}

