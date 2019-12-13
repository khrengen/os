#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	
#include <sys/wait.h>
#include<sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 2) {
        fprintf(stderr, "invalid input\n");
        exit(2);
    }
    struct stat statbuf; 
    pid_t pid;
    int status;
    
    int fd = open(argv[1], O_RDWR | O_CREAT);
    if (fd == -1){
        fprintf(stderr, "can`t open file\n");
    }

    if ( fstat(fd, &statbuf) < 0 ) {

        fprintf(stderr,"fstat error\n");
    }


    char* fmp = (char*)mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
     if (fmp == MAP_FAILED){
        fprintf(stderr,"error mmap");
    }

    if(close(fd) == -1){
        fprintf(stderr, "can`t close file\n");
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "prosses not created\n");
        exit(1);
    }else if (pid == 0){

        char a[255] = {0};
        char b[255] = {0};
        int n = 0;
        if(fmp[0] == '.') {
            fprintf(stderr, "can`t start with '.'\n");
            munmap(fmp, statbuf.st_size);
            exit(6);
        }
        while (fmp[n] != '\n' && fmp[n] != EOF)  {
            if (n == 256) {
                fprintf(stderr, "invalid comand name\n");
                munmap(fmp, statbuf.st_size);
                exit(4);
            }
            if (fmp[n] == '/') {
                fprintf(stderr, "can`t use '/' in names\n");
                 munmap(fmp, statbuf.st_size);
                exit(5);
            }
            a[n]= fmp[n];
            n++;
        }
        n++;
        int k = 0;
        if(fmp[n] == '.') {
            fprintf(stderr, "name can`t start with '.'\n");
            munmap(fmp, statbuf.st_size);
            exit(6);
            }
        while (fmp[n] != '\n' && fmp[n] != EOF)  {
            if (k == 256) {
                fprintf(stderr, "invalid file name\n");
                munmap(fmp, statbuf.st_size);
                exit(4);
            }
            if (fmp[n] == '/') {
                fprintf(stderr, "can`t use '/' in file names\n");
                exit(5);
            }
            b[k]= fmp[n];
            n++;
            k++;
        }
    
        
        if (execlp(a, a, b, (char*)NULL) == -1){
            fprintf(stderr, "exec eror\n");
        }
        
    }else if(pid > 0){
        if (waitpid(pid, &status, 0) == -1){
            fprintf(stderr, "waitpid error\n");
        }

        munmap(fmp, statbuf.st_size);
    }
    return 0;
}