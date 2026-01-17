#include<stdio.h> // scanf printf
#include<stdlib.h> // exit
#include<unistd.h> // fork
#include<sys/wait.h> // waitpid
// usr/include/x86-64-linux/bits/waitstatus.h

int main(){
    
	pid_t pid = fork();
    
	if (pid != 0){
        // pid = child's PID -> its a parent, so we will wait
        int status;
        
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));     // 0 -> for current process
        pid_t finished_pid = waitpid(pid, &status, 0);// 0, WHOHANG, WUNTRACED, WCONTINUED
        if (finished_pid == -1){
            perror("waiting id failed");
        }else{
            printf("Child %d finished, his exit status is %d\n",
                    finished_pid, status);
        }
	}else{
        // pid = 0
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        printf("Child working...\n");
        sleep(3);
        printf("Child done.\n");
        exit(127);
	}
	return 0;
}
