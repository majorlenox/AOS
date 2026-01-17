#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int n = 100;

void sHandler(int sig){
    printf("Got signal %d\n", sig);
    n = 200;
}

int main() {
    int status;
    pid_t pid = fork();
    if (pid != 0) {
        // Parent
        sleep(1);
        kill(pid, SIGUSR1);
        wait(&status);
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)){ // got signal
            printf("Child got signal %d\n", WTERMSIG(status));
        }
    } else {
        // Child
        signal(SIGUSR1, sHandler);
        for (int i = 0; i < n; ++i){
            printf("Iteration: %d\n", i);
            usleep(100*1000);
        }
    }
    return 0;
}
