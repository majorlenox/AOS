#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid != 0) {
        // Parent
        sleep(1);
        kill(pid, SIGCONT);
        wait(&status);
        if (WIFEXITED(status)){
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)){ // got signal
            printf("Child got signal %d\n", WTERMSIG(status));
        }
    } else {
        // Child
        
        // man 7 signal - list of signals and their DFL
        // DFL reaction on SIG
        // terminate
        // ignore
        // stop/continue

        pause(); // return -1 and errno = EINTR
        // ends only if we got terminate or sig with handler
    }
    return 0;
}
