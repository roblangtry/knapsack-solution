#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

#define THREADS 32
#define BATCH_SIZE 100000
#define EMPTY -1
#define END -2
#define TRUE 1
#define FALSE 0


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

/* from https://stackoverflow.com/questions/3437404/min-and-max-in-c */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct {
    int value;
    int weight;
} object_t;

void initialise(int argc, char *argv[], int *size, FILE ** fp);
void process_objects(int map_size, int * map, FILE * fp);
void sequential_process_objects(int map_size, int * map, FILE * fp);
int read_batch(object_t * batch, FILE * fp);
void map_batch(object_t * batch, int batch_size, int ** pmaps, int * map, int map_size);
void consolidate_batch(int ** pmaps, int * map, int map_size);
int parrallel_process_objects(int map_size, FILE * fp);


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
    printf("%d\n", parrallel_process_objects(knapsack_size, fp));
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


int parrallel_process_objects(int map_size, FILE * fp)
{
    LOG("INFO: ");
    LOG_INT(THREADS);
    LOG(" threads requested using parrallel solution\n");
    /* Knapsack solution from https://en.wikipedia.org/wiki/Knapsack_problem#0.2F1_knapsack_problem */
    int value, weight, pval = 10,i,o;
    object_t * objects = malloc(sizeof(object_t)*BATCH_SIZE);
    object_t object;
    int list_size = BATCH_SIZE;
    int occupancy = 0;
    int ** table;
    int row,col;
    while(fscanf(fp, "%d %d", &object.value, &object.weight)>0)
    {
        /* Check the value is valid */
        if(object.weight <= 0 || object.weight > map_size)
        {
            /*
            The value isn't valid either because:
                * it is zero or smaller
                * it is larger than the knapsack size and therefore won't fit
             */
            LOG("WARNING: Invalid object with value \'");
            LOG_INT(object.value);
            LOG("\' of weight \'");
            LOG_INT(object.weight);
            LOG("\' read from input.\n");
        }
        else
        {
            /* The value is valid add it to the objects map*/
            if(pval)
            {
                /* Some logging to allow debugging */
                LOG("INFO: Object with value \'");
                LOG_INT(object.value);
                LOG("\' of weight \'");
                LOG_INT(object.weight);
                LOG("\' read from input.\n");
                pval--;
                if(!pval)
                    LOG("INFO: Further input will be supressed...\n");

            }
            objects[occupancy++] = object;
            if(occupancy == list_size)
            {
                list_size = list_size << 1;
                objects = realloc(objects, list_size*sizeof(object_t));
            }
        }
    }
    table = malloc(occupancy*sizeof(int *));
    for(i=0;i<occupancy;i++){
        table[i]=malloc((1+map_size)*sizeof(int));
    }
    for(row=0;row<occupancy;row++){
        weight = objects[row].weight;
        value = objects[row].value;
        if(!row)
        {
            for(col=0;col<weight;col++){
                table[row][col]=0;
            }
            for(col=weight;col<map_size;col++){
                table[row][col]=value;
            }
        }
        else
        {
            for(col=0;col<weight;col++){
                table[row][col]=table[row-1][col];
            }
            for(col=weight;col<map_size;col++){
                table[row][col]=MAX(table[row-1][col],value+table[row-1][col-weight]);
            }
        }
    }
    return table[occupancy-1][map_size-1];
}