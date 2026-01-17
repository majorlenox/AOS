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
    int fds[2];
    pid_t pid;
    pipe(fds); 
    if (pid = fork() == 0){ // child
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);
        execlp("who", "who", NULL);
    }
    // wait(NULL);
    if (fork() == 0){
        close(fds[1]);
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);
        execlp("wc", "wc", "-l", NULL);       
    }
    close(fds[0]);
    close(fds[1]); // close pipe before wait
    wait(NULL);
    wait(NULL);
    return 0;
}
