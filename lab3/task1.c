#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sHandler(int sig){
    printf("Got signal: %d\n", sig);
    signal(sig, SIG_DFL);
}

int main(){
    signal(SIGTERM, sHandler);   
    while (1){
        pause();
    }
    return 0;
}
