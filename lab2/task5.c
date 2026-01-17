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
        sleep(5);
        // There is a Zombie on your lawn
        printf("Even so child done, we can find him by: ps aux | grep %d\n"
               "or with command: ps -o pid,ppid,state,comm -p %d\n", pid, pid);
        sleep(40); // waiting to see zombie
        printf("Parent ended\n");   
        //  You can kill zombie by:
        //  1. wait in parent 
        //  2. ignore SIGCHLD (SIG_IGN) / or make a handler for it
        //  3. kill parent (now its a systemd's child)
	}else{
        // pid = 0
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        printf("Child working...\n");
        sleep(3);
        printf("Child done.\n");
        exit(127);
        // After exit in table of process (deque btw) will be saved:
        // PID, exit status, {CPU time, memory, etc} and UID, GID
	}
	return 0;
}
