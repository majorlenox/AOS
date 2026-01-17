#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<time.h>
#include<errno.h>

#define BUFFSIZE 10

void try_read(int flags){
    int fd, bytes_read;
    char buffer[BUFFSIZE];
    fd = open("fifo", O_RDONLY | flags); 
    if (fd == -1) {perror("open"); return;}
    bytes_read = read(fd, buffer, BUFFSIZE-1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("read -> %s\n", buffer);
    }else{
        printf("read -> EOF\n");
    }
    close(fd);
    return;
}

void try_write(int flags){
    int fd, bytes;
    char buffer[BUFFSIZE];
    fd = open("fifo", O_WRONLY | flags); 
    if (fd == -1) {perror("open"); return;};
    printf("Send some message to fifo:\n");
    fgets(buffer, BUFFSIZE - 1, stdin);

    bytes = write(fd, buffer, strlen(buffer));
    if (bytes > 0) {
        printf("Sended\n");
    }else{
        perror("try_write");
    }
    close(fd);
    return;


}

int main(){
    char c = '1';

    if (mkfifo("fifo", 0666) == -1){
        if (errno != EEXIST){
            perror("mkfifo");
            exit(1);
        }
    }
     
    while (c != '0'){
        printf("Choose what to do with fifo:\n 0 - Quit\n 1 - read\n 2 - read (non-block)\n 3 - write\n 4 - write (non-block)\n");
        c = getchar();
        getchar(); // \n
        switch (c){
            case '1':
                try_read(0);
            break;
            case '2':
                try_read(O_NONBLOCK); // O_NDELAY
            break;
            case '3':
                try_write(0);
            break;
            case '4':
                try_write(O_NONBLOCK);
            break;
        }
    }
    
    return 0;
}
