#include<stdio.h> // scanf printf
#include<unistd.h> // fork
#include<time.h> // current time of excecution, gettimeoftday

int main(){

	struct timespec ts;	

	pid_t pid = fork();
    clock_gettime(CLOCK_REALTIME, &ts);
	if (pid != 0){
        // pid = child's pid -> its a parent
		printf("I am the parent process and my PID is %d\n", getpid());
	}else{
        // pid = 0 -> its a child
		printf("I am the child process, my parent's PID is %d and my PID is %d\n",
                getppid(), getpid());
	}
    printf("Current time for PID=%d: %ld seconds and %ld nanoseconds\n",
            getpid(), ts.tv_sec, ts.tv_nsec);
	return 0;
}
