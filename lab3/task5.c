#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid != 0) {
        // Parent
        sleep(1); // child should start
        kill(pid, SIGUSR1); // by default terminates the process
        wait(&status);
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)){ // got signal
            printf("Child got signal %d\n", WTERMSIG(status));
        }
    } else {
        // Child
        for (int i = 0; i < 100; ++i){
            printf("Iteration: %d\n", i);
            usleep(100*1000); // microseconds
        }
    }
    return 0;
}
