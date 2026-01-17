#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sHandler(int sig){
    printf("Got signal: %d\n", sig);
}

int main(){
    struct sigaction sa;
    sa.sa_handler = sHandler;
    sigemptyset(&sa.sa_mask); // no additional signals
    sa.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sa, NULL); // reset handler to DFL after one invoc  
    while (1){
        pause();
    }
    return 0;
}
