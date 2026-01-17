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
#define SERVER_KEY 12345

typedef struct our_msgbuf {
    long mtype;         
    int  receiver_pid;  
    int  sender_pid;      
    char name[NAME_LENGTH];
    char data[BUFFSIZE];   
} Message;

int running = 1;

void handler(int sig) {
    running = 0;
}

int main() {
    int msqid, nk_size = 0, f;
    int sender_pid_list[NETWORK_SIZE];
    Message msg;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    msqid = msgget(SERVER_KEY, IPC_CREAT | 0666);
    
    printf("Server started:\n");
    printf("  Queue ID: %d\n", msqid);
    printf("  Server PID: %d\n\n", getpid());

    while (running) {

        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), -2, 0) == -1) { // type 1 and 2 -> to server
            if (running)
                perror("msgrcv");
            continue;
        }

        printf("Received from %d (receiver %d): %s\n", msg.sender_pid, msg.receiver_pid, msg.data);

        if (msg.mtype == 1) { // broadcast
            printf("Broadcasting from %s (pid=%d)\n", msg.name, msg.sender_pid);
            
            for (int i = 0; i < nk_size; ++i) {
                if (sender_pid_list[i] != -1 && sender_pid_list[i] != msg.sender_pid) {
                    msg.mtype = sender_pid_list[i];
                    if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                        perror("msgsnd broadcast error");
                    }
                }
            }
        } else { // unicast or registration
            if (msg.receiver_pid == msg.sender_pid) { // gratuitous -> registration
                if (strcmp(msg.data, "-1") != 0){ // disconnect
                printf("Registration from pid=%d\n", msg.sender_pid);
                
                f = -1;
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == msg.sender_pid) {
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
                    sender_pid_list[f] = msg.sender_pid;
                    printf("Client %d registered\n", msg.sender_pid);
                }
            } else { 
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == msg.sender_pid) {
                        sender_pid_list[i] = -1;
                        printf("Client %d disconnected\n", msg.sender_pid);
                    }
                }
            }} else { // unicast 
                int target_pid = msg.receiver_pid;
                f = 0;
                
                for (int i = 0; i < nk_size; ++i) {
                    if (sender_pid_list[i] == target_pid) {
                        f = 1;
                        msg.mtype = target_pid;
                        if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
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
    
    msgctl(msqid, IPC_RMID, NULL);
    printf("\nServer is shutting down...\n");
    return 0;
}
