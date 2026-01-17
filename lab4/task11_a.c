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
    char buffer[BUFFSIZE];
    int bytes_read;
    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
   
    lock_in(STDIN_FILENO, F_WRLCK);
    printf("Reader %s is reading\n", argv[1]);
    
    while (fgets(buffer, BUFFSIZE-1, stdin) != NULL){
        buffer[strlen(buffer)-1] = '\0';
        printf("Reader %s read:\n%.*s\n----------\n", argv[1], bytes_read, buffer);
        lock_in(STDIN_FILENO, F_UNLCK);
        usleep(1);
        lock_in(STDIN_FILENO, F_WRLCK);
        printf("Reader %s is reading\n", argv[1]);
    }
    lock_in(STDIN_FILENO, F_UNLCK);
    return 0;
}
