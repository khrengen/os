#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "q.h"

#define MAX_SIZE 100
#define LIB_IN_USE "../libq.so"

int dl_error() {
    if (dlerror() != NULL) {
        fprintf(stderr, "dlsym error\n");
        return 1;
    }
    return 0;
}

void exec_method(char *method, queue* q, QueueElemType argument) {
    void* dl_handle;
    fprintf(stdout, "trying to do \"%s\"\n", method);
    dl_handle = dlopen(LIB_IN_USE, RTLD_LAZY);

    if (!dl_handle) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return;
    } 

    if (strcmp(method, "qCreate") == 0) {
        fprintf(stdout, "creating...\n");
        void (*func)(queue*);
        func = dlsym(dl_handle, method);
        if (dl_error() == 0) {
            (*func)(q);
        }
    } else if (strcmp(method, "qPush") == 0) {
        fprintf(stdout, "pushing...\n");
        int (*func)(queue*, QueueElemType);
        func = dlsym(dl_handle, method);
        if (dl_error() == 0){
            if ((*func)(q, argument)) {
                fprintf(stderr, "queue is full\n");
            }
        }

    } else if (strcmp(method, "qPop") == 0) {
        fprintf(stdout, "popping...\n");
        int (*func)(queue*);
        func = dlsym(dl_handle, method);
        if (dl_error() == 0) {
            if ((*func)(q)) {
                fprintf(stderr, "queue is empty\n");
            }
        }

    } else if (strcmp(method, "printq") == 0) {
        void (*func)(queue*, FILE*);
        func = dlsym(dl_handle, method);
        if (dl_error() == 0) {
            (*func)(q, stdout);
        }

    } else {
        fprintf(stderr, "invalid input\n");
    }
 
    dlclose(dl_handle);
 
    return;
}

int main() {
    char buf[MAX_SIZE];
    char func_name[MAX_SIZE];
    int argument;

    queue q;
    for (;;) {
        fprintf(stdout, "enter function: ");
        fgets(buf, MAX_SIZE, stdin);
    
        if (!strncmp(buf, "end", 3)) {
            break;
        }

        sscanf(buf, "%s %d", func_name, &argument);
        exec_method(func_name, &q, argument);
    }
}