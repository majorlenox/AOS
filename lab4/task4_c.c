#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

#define BUFFSIZE 1024

int main(int argc, char* argv[]){
    char buffer[BUFFSIZE];
    int bytes_read, fd;
    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &fd);   
    while((bytes_read = read(STDIN_FILENO, buffer, BUFFSIZE)) > 0){
        write(fd, buffer, bytes_read);
    }
    close(fd);
    return 0;
}
