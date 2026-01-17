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
    int bytes_read, fd, i = 0;
    if (argc != 2){
        printf("Incorrect usage\n");
        exit(1);
    }
    //    sleep(1); // wait to avoid EOF
    fd = open(argv[1], O_RDONLY);
    lock_in(fd, F_RDLCK);
    while((bytes_read = read(fd, buffer, BUFFSIZE-1)) >= 0 && i < 10000){ // read blocking only for special filetypes (pipes, fifo, sockets, terminals)
        lock_in(fd, F_UNLCK);
        if (bytes_read > 0){
            buffer[bytes_read] = '\0';
            printf("Reader read:\n%.*s --------\n", bytes_read, buffer);
        }
   //     usleep(delay*1000);
        i++;
        lock_in(fd, F_RDLCK);
    }
    close(fd);
    return 0;
}
