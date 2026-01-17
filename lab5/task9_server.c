#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define NAME_LENGTH 32
#define BUFFSIZE 1024
#define NETWORK_SIZE 1024
#define SERVER_REQ_KEY 12345   // requests to server
#define SERVER_RES_KEY 12346   // responses from server

typedef struct our_msgbuf {
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
    int req_msqid, res_msqid, nk_size = 0, f;
    long sender_pid_list[NETWORK_SIZE];
    Message msg;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    req_msqid = msgget(SERVER_REQ_KEY, IPC_CREAT | 0666);
    res_msqid = msgget(SERVER_RES_KEY, IPC_CREAT | 0666);
    
    printf("Server started:\n");
    printf("  Request queue ID: %d\n", req_msqid);
    printf("  Response queue ID: %d\n", res_msqid);
    printf("  Server PID: %d\n\n", getpid());

    while (running) {

        if (msgrcv(req_msqid, &msg, sizeof(msg) - sizeof(long), 0, 0) == -1) {
            if (running)
                perror("msgrcv");
            continue;
        }

        printf("Received from %d (mtype %ld): %s\n", msg.sender_pid, msg.mtype, msg.data);

        if (msg.mtype == 1) { // broadcast
            printf("Broadcasting from %s (pid=%d)\n", msg.name, msg.sender_pid);
            
            for (int i = 0; i < nk_size; ++i) {
                if (sender_pid_list[i] != -1 && sender_pid_list[i] != msg.sender_pid) {
                    msg.mtype = sender_pid_list[i];
                    if (msgsnd(res_msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                        perror("msgsnd broadcast error");
                    }
                }
            }
        } else { // unicast or registration
            if (msg.mtype == msg.sender_pid) { // gratuitous -> registration
                if (strcmp(msg.data, "-1") != 0){ // disconnect
                printf("Registration from pid=%d\n", msg.sender_pid);
                
                f = -1;
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == msg.mtype) {
                        f = -2; 
                        break;
                    }
                    if (sender_pid_list[i] == -1 && f == -1) {
                        f = i; 
                    }
                }
                
                if (f == -1) { 
                    f = nk_size;
                    nk_size++;
                }
                
                if (f != -2) {
                    sender_pid_list[f] = msg.mtype;
                    printf("Client %d registered\n", msg.sender_pid);
                }
            } else { 
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == msg.mtype) {
                        sender_pid_list[i] = -1;
                        printf("Client %d disconnected\n", msg.sender_pid);
                    }
                }
            }} else { // unicast 
                int target_pid = msg.mtype;
                f = 0;
                
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == target_pid) {
                        f = 1;
                        msg.mtype = target_pid;
                        if (msgsnd(res_msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                            perror("msgsnd unicast error");
                        }
                        break;
                    }
                }
                
                if (f == 0) {
                    printf("No user with pid=%d\n", target_pid);
                }
            }
        }
    }
    
    msgctl(req_msqid, IPC_RMID, NULL);
    msgctl(res_msqid, IPC_RMID, NULL);
    printf("\nServer is shutting down...\n");
    return 0;
}
