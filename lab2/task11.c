#include<stdio.h>      // scanf printf
#include<stdlib.h>     // exit
#include<unistd.h>     // pause
#include<fcntl.h>      // open
#include<sys/wait.h>   // wait
#include<string.h>

int main(int argc, char* argv[], char* envp[]){
    // usage: ./task11 ls 
    //        ./task11 cp file.txt file1.txt
    // Using system
    printf("Using system:\n");
    char cmd[1024] = {0};
    for(int i = 1; i < argc; i++) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    system(cmd);
    // Using fork-exec
    printf("\nUsing fork-exec:\n");
    pid_t pid = fork();
    if(pid == 0) {
        execvp(argv[1], &argv[1]); // finds in PATH, &argv[1] - all remaining args
    } else if(pid > 0) {
        wait(NULL); // blocks until any children ended
    } else {
        perror("fork");
    }
	return 0;
}
