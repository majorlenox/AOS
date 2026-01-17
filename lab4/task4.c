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

    pipe(fds1); // c1 to c2
  
    pid_t pid = fork();
    if (pid != 0){
        // c2
        close(fds1[1]);
        while ((bytes_read = read(fds1[0], buffer, BUFFSIZE-1)) > 0){
            buffer[bytes_read] = '\0';
            printf("%.*s\n", bytes_read, buffer);
        }
        close(fds1[0]);
    }else{
        // c1
        close(fds1[0]);
        char arg[12];
        sprintf(arg, "%d", fds1[1]);
        execl("./task4_c", "task4_c", arg, NULL);
        perror("execl");
        exit(1);
    }
    return 0;
}
