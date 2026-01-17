#include<stdio.h> // scanf printf
#include<stdlib.h> // exit
#include<unistd.h> // pause

int main(){
    
	pid_t pid = fork();
   
    // to get uid:                  id -u
    // gets all your processes:     ps -u uid/name
    // current terminal:            ps -o pid,ppid,pgid,tpgid,comm -p &&
    // 
    
    // 1. ./task6 &, jobs, kill -9 parent_pid -> child goes to systemd
    // 2. CTRL+C in interactive mode -> all processes killed because SIGINT goes to 
    //    group of foreground processes (PGID==TPGID)
    // 3. CTRL+C after end of parents routine -> doesn't happens anything, because
    //    now TPGID not equal PGID of child -> therefore foreground group is now on bash
	if (pid != 0){
        int status;
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0)); 
        pause(); // 1st,2nd experiments
        // sleep(2);   // 3rd experiment
	}else{
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));
        pause();
	}
	return 0;
}
