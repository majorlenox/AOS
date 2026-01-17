#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>

#define BUFFSIZE 1024
#define delay 4


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
    int bytes, fd, i = 0, num;
    srand(time(NULL));

    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
    fd = open(argv[1], O_WRONLY, 0644);
    strcpy(buffer, "Writer started:\n");
    lock_in(fd, F_WRLCK);
    while((bytes = write(fd, buffer, strlen(buffer))) > 0 && i < 100){
        lock_in(fd, F_UNLCK);
        if (bytes > 0){
            printf("Writer wrote: %.*s", bytes, buffer);
        }else{
            printf("Can't write\n");
        }
        num = rand()%9000 + 1000;
        sprintf(buffer, "Next number is: %d\n", num);
//        usleep(delay*1000);
        i++;
        lock_in(fd, F_WRLCK);
    }
    close(fd);
    return 0;
}
