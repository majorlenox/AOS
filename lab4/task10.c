#define _GNU_SOURCE // because pipe2 from BSD

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>

int main(){

    if (fork() == 0){
        // a1
        execl("./task10_a", "task10_a", "1", NULL);
        perror("execl");
        exit(1);
    }

    if (fork() == 0){
        // writer
        execl("./task10_a", "task10_a", "2", NULL);
        perror("execl");
        exit(1);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}
