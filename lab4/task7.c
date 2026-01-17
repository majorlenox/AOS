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
    int bytes_read, num, fd;

    fd = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    close(fd);

    if (fork() == 0){
        // reader
        execl("./task7_reader", "task7_reader", "file.txt", NULL);
        perror("execl");
        exit(1);
    }

    if (fork() == 0){
        // writer
        execl("./task7_writer", "task7_writer", "file.txt", NULL);
        perror("execl");
        exit(1);
    }

    wait(NULL);
    wait(NULL);
    return 0;
}
