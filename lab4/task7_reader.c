#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

#define BUFFSIZE 1024
#define delay 5

int main(int argc, char* argv[]){
    char buffer[BUFFSIZE];
    int bytes_read, fd, i = 0;
    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
    // sleep(1); // wait to avoid EOF
    fd = open(argv[1], O_RDONLY);
    while((bytes_read = read(fd, buffer, BUFFSIZE-1)) >= 0 && i < 1000){ // read blocking only for special filetypes (pipes, fifo, sockets, terminals)
        if (bytes_read != 0){
            buffer[bytes_read] = '\0';
            printf("Reader read:\n%.*s-------\n",bytes_read, buffer);
        }
//        usleep(delay*1000);
        i++;
    }
    close(fd);
    return 0;
}
