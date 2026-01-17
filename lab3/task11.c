#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t child_pid;
pid_t parent_pid;
volatile sig_atomic_t received = 0; // doesn't shared

void p_sHandler(int sig, siginfo_t *info, void *context){
    if (sig == SIGUSR1 && info->si_pid == child_pid){
        printf("Parent received SIGUSR1 from child\n");
        received = 1;
    }
}

void c_sHandler(int sig, siginfo_t *info, void *context){
    if (sig == SIGUSR1 && info->si_pid == parent_pid){
        printf("Child received SIGUSR1 from parent\n");
        received = 1;
    }
}

void progress_bar(int sz){
        for (int i = 0; i < sz; ++i){
            printf("\r[");
            for (int j = 0; j < sz; ++j){
                if (j <= i){printf("=");}else {printf(" ");}
            }
            printf("]");
            fflush(stdout);
            usleep(100*1000);
        }
}

int main() {

    struct sigaction sa;
    int status;
    sigset_t mask, waiting_mask, old_mask;
    child_pid = fork();
    if (child_pid != 0) {
        // Parent
        sa.sa_sigaction = p_sHandler;
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &sa, NULL);

        // blocking SIGUSR1 
        sigemptyset(&mask);      
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, &old_mask);

        // for sigsuspend
        sigfillset(&waiting_mask);
        sigdelset(&waiting_mask, SIGUSR1);

        // do something
        //sleep(20);        
        progress_bar(50);

        printf(" Parent ready to receive SIGUSR1.\n");
        
        received = 0;
        while (!received)
            sigsuspend(&waiting_mask); // waiting to receive SIGUSR1
        
        sigprocmask(SIG_SETMASK, &old_mask, NULL);

        kill(child_pid, SIGUSR1); // send SIGUSR1

        printf("Parent synchronized\n");
        wait(NULL);
    } else {
        // Child
        parent_pid = getppid();
        sa.sa_sigaction = c_sHandler;
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &sa, NULL);

        // blocking SIGUSR1 
        sigemptyset(&mask);      
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &mask, &old_mask);

        // for sigsuspend
        sigfillset(&waiting_mask);
        sigdelset(&waiting_mask, SIGUSR1);
      
        // doing something (progress bar)
        progress_bar(30);    
        // sleep(10);

        printf(" Child ready. Sending SIGUSR1\n");
       
        kill(parent_pid, SIGUSR1);

        received = 0;
        while (!received)
            sigsuspend(&waiting_mask);  

        printf("Child synchronized.\n");
    }
    return 0;
}
