#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>

#define THREADS 4
#define BATCH_SIZE 100000
#define EMPTY -1
#define END -2
#define TRUE 1
#define FALSE 0
#define NUM_REVIEW 10 /* The number of read items to review */

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
object_t * read_objects(int * size, int map_size, FILE * fp);
int parrallel_process_objects(int map_size, FILE * fp);


int
main(int argc, char *argv[])
{
    int knapsack_size, rank, value;
    FILE * fp = stdin;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    initialise(argc, argv, &knapsack_size, &fp);
    LOG("INFO: Knapsack size - ");
    LOG_INT(knapsack_size);
    LOG("\n");
    value = parrallel_process_objects(knapsack_size, fp);
    if(!rank)
        printf("%d\n", value);
    MPI_Finalize();
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

object_t * read_objects(int * size, int map_size, FILE * fp)
{
    int pval = NUM_REVIEW;
    object_t * objects = malloc(sizeof(object_t)*BATCH_SIZE);
    object_t object;
    int list_size = BATCH_SIZE;
    int occupancy = 0;
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
    *size = occupancy;
    return objects;
}

int get_bounds(int * upper, int * lower, int proc_low, int proc_high)
{
    if(*upper<proc_low)
        *upper = proc_low;
    if(*lower<proc_low)
        *lower = proc_low;
    if(*upper>proc_high)
        *upper = proc_high;
    if(*lower>proc_high)
        *lower = proc_high;
    return 0;
}

int parrallel_process_objects(int map_size, FILE * fp)
{
    LOG("INFO: ");
    LOG_INT(THREADS);
    LOG(" threads requested using parrallel solution\n");
    /* Knapsack solution from https://en.wikipedia.org/wiki/Knapsack_problem#0.2F1_knapsack_problem */
    int i;
    int ** table;
    int num_rows;
    object_t * objects = read_objects(&num_rows, map_size, fp);
    table = malloc(2*sizeof(int *));
    for(i=0;i<2;i++){
        table[i]=calloc(map_size + 1, sizeof(int));
    }
    int col;
    int row;
    int flux;
    int final;
    int value;
    int weight;
    int prev;
    int lower,upper;
    int rank;
    int world_size;
    int target;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &world_size );
    int proc_low = rank * (map_size/world_size);
    int proc_high;
    int send_tag = 0;
    int recv_tag = 0;
    if(rank != world_size-1)
        proc_high = (rank+1) * (map_size/world_size);
    else
        proc_high = map_size+1;

    #pragma omp parallel private(col,row,flux,prev,weight,value,lower,upper) num_threads(THREADS)
    {
        int delta;
        int ndelta;
        flux=1;
        weight=map_size;
        for(row=0;row<num_rows;row++){
            flux = !flux;
            delta = -1;
            prev = weight;
            weight = objects[row].weight;
            value = objects[row].value;
            lower = prev;
            upper = weight;
            get_bounds(&upper, &lower, proc_low, proc_high);;
            #pragma omp for nowait
            for(col=lower;col<upper;col+=1){
                if(delta == -1)
                    delta = col;
                table[flux][col]=table[!flux][col];
            }

            lower = weight;
            upper = map_size + 1;
            get_bounds(&upper, &lower, proc_low, proc_high);
            #pragma omp for nowait
            for(col=lower;col<upper;col+=1){
                if(delta == -1)
                    delta = col;
                table[flux][col]=MAX(table[!flux][col],value+table[!flux][col-weight]);
            }
            #pragma omp barrier
            #pragma omp single
            {
                if(rank){
                    MPI_Recv(&ndelta,1,MPI_INT,rank-1,recv_tag++,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    if(ndelta != -1){
                        delta = ndelta;
                        MPI_Recv(table[flux]+delta,(map_size/world_size * rank)-delta,MPI_INT,rank-1,recv_tag++,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                    }
                }
                if(rank != world_size-1){
                    target = rank+1;
                    MPI_Send(&delta,1,MPI_INT,target,send_tag++,MPI_COMM_WORLD);
                    if(delta != -1){
                        MPI_Send(table[flux]+delta,(map_size/world_size * (rank+1))-delta,MPI_INT,target,send_tag++,MPI_COMM_WORLD);
                    }
                }
            }
        }
        #pragma omp single
        {
            final=flux;
        }
    }
    if(rank == world_size-1){
        MPI_Send(&table[final][map_size],1,MPI_INT,0,0,MPI_COMM_WORLD);
    }
    if(rank == 0){
        MPI_Recv(&table[final][map_size],1,MPI_INT,world_size-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    return table[final][map_size];
}