#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc != 3) {
        fprintf(stderr, "invalid input\n");
        exit(2);
    }
    pid_t pid;
    int status;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "prosses not created\n");
        exit(1);
    }else if (pid == 0){
        int fd = open(argv[2], O_RDWR | O_CREAT);
        if (fd == -1){
            fprintf(stderr, "can`t open file\n");
        }
        if(dup2(fd, 0) == -1){
            fprintf(stderr, "dup eror\n");
        }
        if (execlp(argv[1], argv[2], NULL) == -1){
            fprintf(stderr, "exec eror\n");
        }
        if(close(fd) == -1){
            fprintf(stderr, "can`t close file\n");
        }
    }else if(pid > 0){
        if (waitpid(pid, &status, 0) == -1){
            fprintf(stderr, "smth wrong with parent\n");
        }
    }
    return 0;
}
