#include<stdio.h>      // scanf printf
#include<stdlib.h>     // exit
#include<unistd.h>     // pause
#include<fcntl.h>      // open
#include<sys/wait.h>   // wait

int main(int argc, char* argv[], char* envp[]){
    
    int fd, fd1, bytes, status;
    char *thisEnv;

    char msg[] = "MSG1=Message to my child";
    putenv(msg);

	pid_t pid = fork();
    
	if (pid != 0){
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));    
        printf("Arguments:\n");
        for(int i = 0; i < argc; i++) {
            printf("%d: %s\n", i, argv[i]);
        }
        printf("Environment:\n");
        for (char **env = envp; *env != 0; env++)
        {
            printf("%s\n", *env);    
        }
        printf("Parent ended.\n");
	}else{
        printf("Child before exec: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));    
        char* new_argv[] = {argv[1], "1337", "deadbeef", NULL};
        // exec* functions
        // exec{l/v}, l - list of args (useful when in compile time we know everything about args),
        // v - vector of args (in our case)
        // execp, p - uses PATH enviroment to find the program
        // execpve, e - takes envp from args
        execv(argv[1], new_argv);
        // unreachable
	}
	return 0;
}
