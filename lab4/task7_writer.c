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
    while((bytes = write(fd, buffer, strlen(buffer))) > 0 && i < 100){
        if (bytes > 0){
            printf("Writer wrote: %.*s", bytes, buffer);
        }else{
            printf("Can't write\n");
        }
        num = rand()%9000 + 1000;
        sprintf(buffer, "Next number is: %d\n", num);
 //       usleep(delay*1000);
        i++;
    }
    if (bytes > 0){ // for last one
      printf("Writer wrote: %.*s", bytes, buffer);
    }else{
      printf("Can't write\n");
    }

    close(fd);
    return 0;
}
