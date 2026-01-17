#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>

int m[1024], k = 0;

void seePending(){
     const char *signal_names[] = {
        "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS",
        "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
        "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
        "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM",
        "SIGPROF", "SIGWINCH", "SIGPOLL", "SIGPWR", "SIGSYS"
    };
    sigset_t mask;
    sigpending(&mask);
    for (int sig = 1; sig <= 31; sig++) {
        if (sigismember(&mask, sig)) {
            printf("%s(%d) ", signal_names[sig-1], sig);
        }
    }
    signal(SIGINT, SIG_DFL);
    printf("\n");
}

void sHandler(int sig){
    sigset_t mask, old_mask; // Can't block SIGKILL, SIGSTOP
    sigemptyset(&mask);
    sigfillset(&mask); // all sigs

    sigdelset(&mask, SIGUSR1); // without usr1
    
    sigprocmask(SIG_BLOCK, &mask, &old_mask); // Nichego ne ponimau. Blockiryiu! 
   
    printf("Do some handling with %s\n", strsignal(sig));
    sleep(10);
    printf("Handling done\n");
    seePending();
   // sigprocmask(SIG_SETMASK, &old_mask, NULL); 
}

int main(){
    signal(SIGUSR1, sHandler);
    printf("Send me SIGUSR1: PID=%d\n", getpid());
    while (1) pause();
    
    return 0;
}
