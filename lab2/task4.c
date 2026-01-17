#include<stdio.h> // scanf printf
#include<stdlib.h> // exit
#include<unistd.h> // fork
#include<sys/wait.h> // waitid

int main(){
    
	pid_t pid = fork();
   
	if (pid != 0){
        // pid = child's PID -> its a parent, so we will wait
        int status;
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        sleep(1);
        printf("All those moments will be lost in time, like tears in rain. Time to die\n");
	    exit(0);
    }else{
        // pid = 0
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        sleep(3);
        printf("I am still alive.\n");
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        // PPID=1964 its a systemd     
	}
	return 0;
}
