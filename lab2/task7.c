#include<stdio.h> // scanf printf
#include<stdlib.h> // exit
#include<unistd.h> // pause

int main(){
    
	pid_t pid = fork();
   
	if (pid != 0){
        int status;
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));    
        pause();   
	}else{
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        setpgid(0, 0); // EPERM, ESRCH, EINVAL
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        pause();
	}
	return 0;
}
