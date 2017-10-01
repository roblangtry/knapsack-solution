#include <stdio.h>
#include <stdlib.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#if DEBUG
#define LOG(x) fprintf(stderr, "%s", x)
#define LOG_INT(x) fprintf(stderr, "%d", x)
#else
#define LOG(x)
#define LOG_INT(x)
#endif

void initialise(int argc, char *argv[], int *size, FILE ** fp);

int
main(int argc, char *argv[])
{
    int knapsack_size;
    FILE * fp = stdin;
    initialise(argc, argv, &knapsack_size, &fp);
    LOG("Starting work\n");
    LOG("  knapsack size - ");
    LOG_INT(knapsack_size);
    LOG("\n");
    return 0;
}

void
initialise(int argc, char *argv[], int *size, FILE ** fp)
{
    /* check the number of arguments is correct */
    if(argc != 2 && argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid Syntax.\n");
        fprintf(stderr, "  Correct Syntax : %s size [file]\n", argv[0]);
        fprintf(stderr, "    size - The size of the knapsack\n");
        fprintf(stderr, "    file - The file from which to read object sizes (if not specified object sizes are read from stdin)\n");
        exit(EXIT_FAILURE);
    }
    /* check the size is a number. */
    *size = atoi(argv[1]);
    if(*size <= 0)
    {
        fprintf(stderr, "ERROR: \"%s\" is not a valid size for the knapsack.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    /* check if a file is specified that it is valid. */
    if(argc == 3)
    {
        *fp = fopen(argv[2], "r");
        if(!(*fp)){
            fprintf(stderr, "ERROR: file \"%s\" does not exist.\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }
}