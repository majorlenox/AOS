#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include <signal.h> 
#include <sys/wait.h>

#define BUFFSIZE 1024
#define SERVER_REQ_KEY 12345
#define SERVER_RES_KEY 12346
#define NAME_LENGTH 32

typedef struct {
    long mtype;
    int  sender_pid;
    char name[NAME_LENGTH];
    char data[BUFFSIZE];
} Message;

int running = 1;

void handler(int sig) {
    running = 0;
}

int main() {
    struct sigaction sa;
    int req_msqid, res_msqid;
    
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    
    req_msqid = msgget(SERVER_REQ_KEY, 0666);
    res_msqid = msgget(SERVER_RES_KEY, 0666);
    
    if (req_msqid == -1 || res_msqid == -1) {
        perror("msgget server queues");
        printf("Start server first: ./task9_server\n");
        exit(1);
    }
    
    printf("Client PID = %d connected\n", getpid());
    printf("Request queue: %d, Response queue: %d\n\n", req_msqid, res_msqid);
    
    Message reg_msg;
    reg_msg.mtype = reg_msg.sender_pid = getpid(); // registration
    
    if (msgsnd(req_msqid, &reg_msg, sizeof(reg_msg) - sizeof(long), 0) == -1) {
        perror("msgsnd registration");
        exit(1);
    }
    
    char name[NAME_LENGTH];
    printf("Enter your name: ");
    fgets(name, NAME_LENGTH, stdin);
    name[strlen(name)-1] = '\0';
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // child -> responses
        while (running) {
            Message recv_msg;
            
            if (msgrcv(res_msqid, &recv_msg, sizeof(recv_msg) - sizeof(long), getppid(), 0) == -1) {
                if (running) perror("msgrcv response");
                break;
            }
           
            printf("\nMessage from %s (pid=%d)\n Text: %s\n", recv_msg.name,
                    recv_msg.sender_pid, recv_msg.data);
            printf("Enter message: ");
            fflush(stdout);
        }
        exit(0);
    }
    
    // parent -> requests
    printf("Message format:\n");
    printf("  'message' - broadcast to all\n");
    printf("  '@PID message' - private to PID\n");
    printf("  'CTRL-D' - disconnect\n\n");
    printf("Enter message: ");
    fflush(stdout);
    
    char buffer[BUFFSIZE];
    while (running && fgets(buffer, BUFFSIZE, stdin)) {
        buffer[strlen(buffer)-1] = '\0';
               
        Message send_msg;
        send_msg.sender_pid = getpid();
        strcpy(send_msg.name, name);
        strcpy(send_msg.data, buffer);
        
        if (buffer[0] == '@') {
            int target_pid;
            char *space = strchr(buffer, ' ');
            
            if (space && sscanf(buffer+1, "%d", &target_pid) == 1) {
                send_msg.mtype = target_pid; // unicast
                strcpy(send_msg.data, space+1);
            } else {
                send_msg.mtype = 1; // broadcast
            }
        } else {
            send_msg.mtype = 1; // broadcast
        }
        
        if (strlen(send_msg.data) != 0){
       
        // to requests
        if (msgsnd(req_msqid, &send_msg, sizeof(send_msg) - sizeof(long), 0) == -1) {
            perror("msgsnd request");
        }

        }
        printf("Enter message: ");
        fflush(stdout);
    }
    
    // disconnect
    Message disc_msg;
    disc_msg.mtype = disc_msg.sender_pid = getpid();
    strcpy(disc_msg.name, name);
    strcpy(disc_msg.data, "-1");
    msgsnd(req_msqid, &disc_msg, sizeof(disc_msg) - sizeof(long), 0);
       
    kill(pid, SIGTERM);
    waitpid(pid, NULL, 0);
    
    printf("\nClient PID = %d is shutting down...\n", getpid());
    return 0;
}
