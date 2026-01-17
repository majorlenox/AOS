#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[], char* envp[]){
    printf("Child's exec: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
    printf("Arguments:\n");
    for(int i = 0; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    printf("Environment:\n");
    for(char **env = envp; *env; env++) {
        printf("%s\n", *env);
    }
    printf("Child's exec ended\n");
    return 0;
}
