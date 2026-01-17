#include<stdio.h>      // scanf printf
#include<stdlib.h>     // exit
#include<unistd.h>     // pause
#include<fcntl.h>      // open
#include<sys/wait.h>   // wait

#define BUF_SIZE 100

int main(int argc, char* argv[]){
    
    int fd = open(argv[1], O_RDONLY);
    int fd1, bytes, status;
    char buf[BUF_SIZE];

    if (fd == -1){
        perror("Can't read file");
        return -1;
    }

	pid_t pid = fork();

    // Child and parents shares the same file descriptor
    // child inherits fds, which includes offset and flags
    // also inherits ids, env, umask, dirs, handlers, nice value

	if (pid != 0){
        printf("Parent: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));    
        fd1 = open("parents_file.txt", O_CREAT | O_WRONLY, 0644);   
        while ((bytes = read(fd, buf, BUF_SIZE)) > 0){
             write(fd1, buf, bytes); 
           //  printf("%.*s", bytes, buf);
        }
        if (bytes < 0) perror("Error reading big_file in parent process");
        close(fd1);
        waitpid(pid, 0, 0); // wait for child to end reading
        close(fd);
        fd1 = open("parents_file.txt", O_RDONLY);
        printf("Parent reading:...\n");
        while ((bytes = read(fd1, buf, BUF_SIZE)) > 0){
            printf("%.*s", bytes, buf);
        }
        // putchar('\n');
        if (bytes < 0) perror("Error reading parent_file in parent process");
        close(fd1); 
        printf("Parent ended.\n");
	}else{
        printf("Child: PID=%d, PPID=%d, PGID=%d\n", getpid(), getppid(), getpgid(0));    
        fd1 = open("child_file.txt", O_CREAT | O_WRONLY, 0644);
        while ((bytes = read(fd, buf, BUF_SIZE)) > 0){
           write(fd1, buf, bytes);
           // printf("%.*s", bytes, buf);
        }
        if (bytes < 0) perror("Error reading big_file in child process");
        close(fd1);
        fd1 = open("child_file.txt", O_RDONLY);
        printf("Child reading...\n");
        while ((bytes = read(fd1, buf, BUF_SIZE)) > 0){
            printf("%.*s", bytes, buf);
        }
        // putchar('\n');
        if (bytes < 0) perror("Error reading child_file in child process");
        close(fd1);
        printf("Child ended.\n");
	}
	return 0;
}
