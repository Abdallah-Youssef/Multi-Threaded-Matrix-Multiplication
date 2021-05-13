#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "matrix.h"
#include <sys/time.h>

// used to identify threads
typedef struct element_index{
    int i;
    int j;

} element_index;

matrix a, b, c;
void clean_up()
{
    dealloc_matrix(&a);
    dealloc_matrix(&b);
    dealloc_matrix(&c);
    exit(0);
}


void *thread_per_element_run(void *arg){
    element_index index = *((element_index*) arg);
    int i = index.i;
    int j = index.j;
    int sum = 0;
    for (int k = 0;k < a.m;k++){
        sum += a.data[i][k] * b.data[k][j];
    }

    c.data[i][j] = sum;

}


void *thread_per_row_run(void *arg){
    int i = *((int *)arg);

    for (int j = 0; j < c.m; j++){
        int sum = 0;

        for (int k = 0; k < a.m; k++){
            sum += a.data[i][k] * b.data[k][j];
        }

        c.data[i][j] = sum;
    }
}

int one_thread()
{
    struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time

    for (int i = 0; i < c.n; i++)
    {
        for (int j = 0; j < c.m; j++)
        {
            int sum = 0;

            for (int k = 0; k < a.m; k++)
            {
                sum += a.data[i][k] * b.data[k][j];
            }

            c.data[i][j] = sum;
        }
    }

    gettimeofday(&stop, NULL); //end checking time

    return stop.tv_usec - start.tv_usec;
}

int thread_per_row()
{
    pthread_t threads[a.n];
    int indices[a.n];

    struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
    
    // start n threads. One for each row
    for (int i = 0; i < a.n; i++)
    {
        indices[i] = i;
        pthread_create(&threads[i], NULL, thread_per_row_run, &indices[i]);
    }

    // join threads
    for (int i = 0; i < a.n; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&stop, NULL); //end checking time
    return stop.tv_usec - start.tv_usec;

}

int thread_per_element(){
    pthread_t threads[a.n][b.m];
    element_index indices[a.n][b.m];

     struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
    

    for (int i = 0; i < c.n; i++){
        for (int j = 0;j < c.m;j++){
            // thread for each element in c
            indices[i][j].i = i;
            indices[i][j].j = j;

            pthread_create(&threads[i][j], NULL, thread_per_element_run, &indices[i][j]);
        }
    }

    for (int i = 0; i < c.n; i++)
        for (int j = 0;j < c.m;j++)
            pthread_join(threads[i][j], NULL);


    gettimeofday(&stop, NULL); //end checking time
    return stop.tv_usec - start.tv_usec;

}


int main(int argc, char **argv)
{
    char* a_filename = "a.txt";
    char* b_filename = "b.txt";
    char* c_filename = "c.txt";

    switch (argc)
    {
    case 4:
        // override default filenames
        a_filename = argv[1];
        b_filename = argv[2];
        c_filename = argv[3];
        break;

    case 1:
        // do not override default values
        break;

    default:
        printf("Invalid number of arguments.\n");
        return 22; // invalid argument error code
    }

    if (!init_matrix(&a, a_filename) || !init_matrix(&b, b_filename))  // read matrix from filename
        clean_up(); // if an error occured, clean up by deallocating matrix data (exits automatically)

    if (a.m != b.n){
        printf("Invaild Dimensions. Cannot multiply \n");
        clean_up();
    }

    // set up c
    c.n = a.n;
    c.m = b.m;
    alloc_matrix(&c);


    int t_one_thread = one_thread();
    printf("One thread: %d\n", t_one_thread);
    

    
    int t_per_row = thread_per_row();
    printf("Thread per row: %d\n", t_per_row);

    int t_per_element = thread_per_element();
    printf("Thread per element: %d\n", t_per_element);    


    //write matrix c to c_filename
    write_matrix(&c, c_filename);
    clean_up();
    return 0;
}