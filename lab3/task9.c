#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void sHandler(int sig){
    printf("Just 5 more minutes... (-_-)\n"); // Ignore alarm +- SIGIGN
}

int main() {
    int status;
    pid_t pid = fork();
    if (pid != 0) {
        // Parent
        wait(&status);
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)){ // got signal
            printf("Child got signal %d\n", WTERMSIG(status));
        }
    } else {
        // Child
        // signal(SIGALRM, sHandler);
        alarm(2); // will get SIGALRM after 2 sec
        for (int i = 0; i < 100; ++i){
            printf("Iteration: %d\n", i);
            usleep(100*1000); // microseconds
        }
    }
    return 0;
}
