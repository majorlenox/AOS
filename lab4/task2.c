#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>

#define BUFFSIZE 1024

int main(){
    int fds1[2];
    char buffer[BUFFSIZE];
    int bytes_read, num;

    // pipe(fds1); // child to parent

    pipe2(fds1, O_NONBLOCK);
    // close fd[0] -> read(fd[0]) - EBADF, bad file descr
    // close fd[0] -> read(fd[1]) - returns 0
    // close fd[1] -> write(fd[0]) - SIGPIPE, EPIP
    

    pid_t pid = fork();
    if (pid != 0){
        // Parent
        close(fds1[1]);
        
        while ((bytes_read = read(fds1[0], buffer, BUFFSIZE-1)) > 0){
            printf("Parent received: %.*s", bytes_read, buffer);
        }
        wait(NULL);
        close(fds1[0]);
    }else{
        // Child
        close(fds1[0]);
        while ((bytes_read = read(STDIN_FILENO, buffer, BUFFSIZE-1)) > 0){
//            printf("%.*s\n", bytes_read, buffer);
            write(fds1[1], buffer, bytes_read);
        }
       
        close(fds1[1]);
    }
    return 0;
}
