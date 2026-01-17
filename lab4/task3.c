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
    int fds1[2], fds2[2];
    char buffer[BUFFSIZE];
    int bytes_read, num;

    pipe(fds1); // child to parent
    pipe(fds2); // parent to child    

    // pipe2(fds1, O_NONBLOCK);

    pid_t pid = fork();
    if (pid != 0){
        // Parent
        close(fds1[1]);
        close(fds2[0]);
        
        while ((bytes_read = read(fds1[0], buffer, BUFFSIZE-1)) > 0){
            buffer[bytes_read] = '\0';
            printf("Parent received: %.*s\nParent sends: ", bytes_read, buffer);
            fflush(STDIN_FILENO);
            bytes_read = read(STDIN_FILENO, buffer, BUFFSIZE);
            if (bytes_read <= 0) break;
            write(fds2[1], buffer, bytes_read);
        }
        
        close(fds1[0]);
        close(fds2[1]);
        wait(NULL);
    }else{
        // Child
        close(fds1[0]);
        close(fds2[1]);

        printf("Child writes: ");
        fflush(STDIN_FILENO);
        while ((bytes_read = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0){
            write(fds1[1], buffer, bytes_read);
            bytes_read = read(fds2[0], buffer, BUFFSIZE-1);
            if (bytes_read <= 0) break;
            buffer[bytes_read] = '\0';
            printf("Child received: %.*s\nChild sends: ", bytes_read, buffer);
            fflush(STDIN_FILENO);
        }
               
        close(fds1[1]);
        close(fds2[0]);
    }
    return 0;
}
