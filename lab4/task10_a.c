#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

#define BUFFSIZE 1024
#define delay 5

int lock_in(int fd, int type){
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    return fcntl(fd, F_SETLKW, &lock);
}

int main(int argc, char* argv[]){
    char buffer1[BUFFSIZE];
    char buffer2[BUFFSIZE];
    int bytes_read, i = 0;
    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
   
    // lock_in(STDIN_FILENO, F_RDLCK);
    while((bytes_read = read(STDIN_FILENO, buffer1, BUFFSIZE-1)) >= 0 && i < 10000){
        // lock_in(fd, F_UNLCK);
        if (bytes_read > 0){
            buffer1[bytes_read] = '\0';
            sprintf(buffer2, "Reader %s read:\n%.*s --------\n", argv[1], bytes_read, buffer1);
            write(STDOUT_FILENO, buffer2, strlen(buffer2) + 1);
        }
        // lock_in(fd, F_RDLCK);
    }
    return 0;
}
