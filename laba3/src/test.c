#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

const int MAX_THREAD = 30000;
double count = 0;
int r = 0;
pthread_mutex_t mutex;


void* threadFunc() {
    pthread_mutex_lock(&mutex);
    count++;
    pthread_mutex_unlock(&mutex);
    sleep(10);
   pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "invalid input\n");
        exit(2);
    }

pthread_mutex_init(&mutex, NULL);
pthread_t* threads = (pthread_t*) malloc (atoi(argv[1]) * sizeof(pthread_t));
    
        for(int i = 0; i < atoi(argv[1]); i++) {
            if (pthread_create(&(threads[i]), NULL, threadFunc, NULL) != 0){
                fprintf(stderr, "error pthread creating\n");
               // count++;
                //printf("%d\n", i);
            }
          
        }


        for(int i = 0; i <atoi(argv[1]); i++) {
            pthread_join(threads[i], NULL);
        }


    printf("%f\n", count);

    pthread_mutex_destroy(&mutex);
    free(threads);
    return 0;
}