#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

const int MAX_THREAD = 6800;
double count = 0;
int r = 0;
pthread_mutex_t mutex;

void* threadFunc() {
    srand(pthread_self() + r);
    int a = rand() % 52;
    int b = rand() % 52;
    while(a == b) {
        b = rand() % 52;
    }
    if (a / 4 == b / 4) {
        if(pthread_mutex_lock(&mutex) != 0){
            fprintf(stderr, "error lock mutex\n");
        }
        count++;
        if(pthread_mutex_unlock(&mutex) !=0){
            fprintf(stderr, "error unlock mutex\n");
        }
    }
    r++;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "invalid input\n");
        exit(2);
    }

    if(pthread_mutex_init(&mutex, NULL) != 0){
        fprintf(stderr, "error mutexin\n");
    }
    pthread_t* threads = (pthread_t*) malloc(MAX_THREAD * sizeof(pthread_t));
    double x = 0;
    double n = 0;
    while (n < atoi(argv[1])){
        for(int i = 0; i < MAX_THREAD; i++) {
            if (pthread_create(&(threads[i]), NULL, threadFunc, NULL) != 0){
                fprintf(stderr, "error pthread creating\n");
            }
            x++;
            if(n + x == atoi(argv[1])){
                break;
            }
        }
        n += x;
        for(int i = 0; i < x; i++) {
            if(pthread_join(threads[i], NULL) != 0){
                fprintf(stderr, "error join\n");
            }
        }
        x = 0;
    }
    printf("%f\n", count/atoi(argv[1]));
    if(pthread_mutex_destroy(&mutex) != 0){
        fprintf(stderr, "error mutex destroy\n");
    }
    free(threads);
    return 0;
}
