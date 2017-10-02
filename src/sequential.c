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
void process_objects(int map_size, int * map, FILE * fp);


int
main(int argc, char *argv[])
{
    int knapsack_size;
    int * map;
    FILE * fp = stdin;
    initialise(argc, argv, &knapsack_size, &fp);
    LOG("INFO: Knapsack size - ");
    LOG_INT(knapsack_size);
    LOG("\n");
    map = calloc(knapsack_size + 1, sizeof(int));
    process_objects(knapsack_size, map, fp);
    return 0;
}


void
initialise(int argc, char *argv[], int *size, FILE ** fp)
{
    /* Check the number of arguments is correct */
    if(argc != 2 && argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid Syntax.\n");
        fprintf(stderr, "  Correct Syntax : %s size [file]\n", argv[0]);
        fprintf(stderr, "    size - The size of the knapsack.\n");
        fprintf(stderr, "    file - The file from which to read object sizes (if not specified object sizes are read from stdin).\n");
        exit(EXIT_FAILURE);
    }
    /* Check the size is a number. */
    *size = atoi(argv[1]);
    if(*size <= 0)
    {
        fprintf(stderr, "ERROR: \"%s\" is not a valid size for the knapsack.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    /* Check if a file is specified that it is valid. */
    if(argc == 3)
    {
        *fp = fopen(argv[2], "r");
        if(!(*fp)){
            fprintf(stderr, "ERROR: File \"%s\" does not exist.\n", argv[2]);
            exit(EXIT_FAILURE);
        }
    }
}


void process_objects(int map_size, int * map, FILE * fp)
{
    /* Knapsack solution from https://en.wikipedia.org/wiki/Knapsack_problem#0.2F1_knapsack_problem */
    int value, weight, pval = 10,i;
    while(fscanf(fp, "%d %d", &value, &weight)>0)
    {
        /* Check the value is valid */
        if(weight <= 0 || weight > map_size)
        {
            /*
            The value isn't valid either because:
                * it is zero or smaller
                * it is larger than the knapsack size and therefore won't fit
             */
            LOG("WARNING: Invalid object with value \'");
            LOG_INT(value);
            LOG("\' of weight \'");
            LOG_INT(weight);
            LOG("\' read from input.\n");
        }
        else
        {
            /* The value is valid add it to the objects map*/
            if(pval)
            {
                /* Some logging to allow debugging */
                LOG("INFO: Object with value \'");
                LOG_INT(value);
                LOG("\' of weight \'");
                LOG_INT(weight);
                LOG("\' read from input.\n");
                pval--;
                if(!pval)
                    LOG("INFO: Further input will be supressed...\n");

            }
            /* Do processing */
            for(i = map_size; i>=0;i--)
            {
                if(weight <= i)
                {
                    if(map[i] < map[i-weight]+value)
                        map[i] = map[i-weight]+value;
                }
            }
        }
    }
    printf("%d\n", map[map_size]);
}