#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    pid_t pid = fork();
    if (pid != 0) {
        // Parent
        signal(SIGCHLD, SIG_IGN); 
        printf("Parent %d\n", getpid());
        pause();
        printf("Parent done.\n");
        exit(0);
    } else {
        // Child
        printf("Child %d\n", getpid());
        sleep(2); 
        printf("Child done.\n");
    }
    return 0;
}
