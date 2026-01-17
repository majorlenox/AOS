#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>

#define BUFFSIZE 1024

int main(){
    // deadlock
    int fds1[2], fds2[2]; // in/out
    char buffer[BUFFSIZE];
    int bytes_read, num;

    pipe(fds1); // parent to child
    pipe(fds2); // child to parent

    pid_t pid = fork();
    if (pid != 0){
        // Parent
        close(fds1[0]);
        close(fds2[1]);
        
        bytes_read = read(fds2[0], buffer, BUFFSIZE-1);
        buffer[bytes_read] = '\0';
        printf("Parent received: %s\n", buffer);

        sscanf(buffer, "%*[^0-9]%d", &num);        
        printf("Parent got number: %d\n", num);
        num = num/2;
        
        sprintf(buffer, "I am your parent, My respond is number = %d", num);
        write(fds1[1], buffer, strlen(buffer) + 1);

        wait(NULL);
        close(fds1[1]);
        close(fds2[0]);
    }else{
        // Child
        close(fds1[1]);
        close(fds2[0]);

        srand(time(NULL));
       
        sprintf(buffer, "Hello I am your child, some random number = %d", rand()%9000 + 1000);
        write(fds2[1], buffer, strlen(buffer) + 1);
        
        bytes_read = read(fds1[0], buffer, BUFFSIZE-1);    
        buffer[bytes_read] = '\0';
        printf("Child received: %s\n", buffer);
        sscanf(buffer, "%*[^0-9]%d", &num);

        printf("Child got number: %d\n", num);

        close(fds1[0]);
        close(fds2[1]);
    }
    return 0;
}
