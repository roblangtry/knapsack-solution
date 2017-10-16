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
void parrallel_process_objects(int map_size, int * map, FILE * fp);


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
    free(map);
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

void process_objects(int map_size, int * map, FILE * fp){
    if(THREADS > 1)
        parrallel_process_objects(map_size, map, fp);
    else
        sequential_process_objects(map_size, map, fp);
}

void sequential_process_objects(int map_size, int * map, FILE * fp)
{
    LOG("INFO: ");
    LOG_INT(THREADS);
    LOG(" threads requested using sequential solution\n");
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
            for(i = map_size; i>=weight;i--)
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

int read_batch(object_t * batch, FILE * fp)
{
    int size = 0;
    // /printf("READ\n");
    while(size < BATCH_SIZE){
        if(fscanf(fp, "%d %d", &batch[size].value, &batch[size].weight) != 2){
            // /printf("INC\n");
            return size;
        }
        else{
            // /printf("FUL\n");
            size++;
        }
    }
    return size;
}

void map_object(int weight, int value, int * map, int map_size)
{
    int i;
    for(i = map_size; i >= weight; i--)
    {
        if(weight <= i)
        {
            if(map[i] < map[i-weight]+value)
                map[i] = map[i-weight]+value;
        }
    }
}

void map_batch(object_t * batch, int batch_size, int ** batch_maps, int * map, int map_size)
{
    int t_no,i;
    #pragma omp parallel num_threads(THREADS) private(t_no,i)
    {
        t_no = omp_get_thread_num();
        for(i=t_no; i < batch_size ; i = i + THREADS)
        {
            map_object(batch[i].weight, batch[i].value, batch_maps[t_no], map_size);
        }
    }
}

void reconcile_map(int weight, int value, int * map, int map_size, int * origin)
{
    int i;
    for(i = map_size; i >= weight; i--)
    {
        if(weight <= i)
        {
            if(map[i] < origin[i-weight]+value)
                map[i] = origin[i-weight]+value;
        }
    }
}

void reconcile(int * thread_map, int * map, int map_size)
{
    int j;
    int * shadow = malloc((map_size + 1) * sizeof(int));
    memcpy(shadow, map, (map_size + 1) * sizeof(int));
    for(j=1; j<map_size; j++)
    {
        if(thread_map[j])
            reconcile_map(j, thread_map[j], map, map_size, shadow);
    }
    free(shadow); 
}

void consolidate_batch(int ** batch_maps, int * map, int map_size)
{
    int i;
    for(i=0;i<THREADS;i++)
    {
        reconcile(batch_maps[i], map, map_size);
    }
}

void parrallel_process_objects(int map_size, int * map, FILE * fp)
{
    LOG("INFO: ");
    LOG_INT(THREADS);
    LOG(" threads requested using parrallel solution\n");
    int size,i;
    // /printf("BATCHMAPS\n");
    int ** batch_maps = malloc(sizeof(int *) * THREADS);
    for(i=0;i<THREADS;i++)
    {
        batch_maps[i] = calloc(map_size, sizeof(int));
    }
    // /printf("PREP\n");
    object_t batch[BATCH_SIZE];
    size = read_batch(batch, fp);
    while(size == BATCH_SIZE)
    {
        // /printf("MAP\n");
        map_batch(batch, size, batch_maps, map, map_size);
        size = read_batch(batch, fp);
    }
    if(size)
    {
        // /printf("MAP\n");
        map_batch(batch, size, batch_maps, map, map_size);
    }
    // /printf("CON\n");
    consolidate_batch(batch_maps, map, map_size);
    printf("%d\n", map[map_size]);
}