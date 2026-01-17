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
#define SERVER_KEY 12345   // fixed key for server

typedef struct our_msgbuf {
    long mtype;           
    int  sender_pid;      
    char name[NAME_LENGTH];
    char data[BUFFSIZE];   
} Message;

int running = 1;

void handler(int sig){
     running = 0;
}

int main() {
    int server_msqid, nk_size = 0, f, sender_msqid, receiver_msqid;
    int sender_pid_list[NETWORK_SIZE];
    int sender_msqid_list[NETWORK_SIZE];
    Message msg;
    struct msqid_ds ds;

    struct sigaction sa;

    // catch ctrl+C to shutdown
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    server_msqid = msgget(SERVER_KEY, IPC_CREAT | 0666);
    printf("Server connected to msqid = %d\n", server_msqid);

    printf("server's PID: %d\n", getpid());

    while (running) {

        // Get message with any mtype 
        if (msgrcv(server_msqid, &msg, sizeof(msg) - sizeof(long), 0, 0) == -1) {
            if (running)
                perror("msgrcv");
            continue;
        }

        if (msg.mtype == 1){ // broadcasting
            printf("broadcast from %d\n", msg.sender_pid);
            for (int i = 0; i < nk_size; ++i){
//                printf("out list1 = %d, list2 = %d\n", sender_pid_list[i], sender_msqid_list[i]);
                if (sender_pid_list[i] != -1 && sender_pid_list[i] != msg.sender_pid){ // no self broadcasting
  //              printf("in list1 = %d, list2 = %d\n", sender_pid_list[i], sender_msqid_list[i]);
                    if (msgsnd(sender_msqid_list[i], &msg, sizeof(msg)-sizeof(long), 0) == -1){
                        perror("msgsnd broadcast error");
                    }
                }
            }

           
        }else{ // unicasting

            if (msg.mtype == msg.sender_pid){ // gratuitous
                sscanf(msg.data, "%d", &sender_msqid);
                printf("gratuitous from %ld with data = %d\n", msg.mtype, sender_msqid);
                if (sender_msqid != -1){
                    f = -1;
                    for (int i = 0; i < nk_size; ++i){
                        if (sender_pid_list[i] == msg.sender_pid){ // already in list
                            f = -2;
                            break;
                        }else{
                            if (sender_pid_list[i] == -1 && f == -2) // first -1 -> deleted msqid
                                f = i;
                        }
                    }
                    if (f == -1){ // write to the end of the list
                        f = nk_size;
                        nk_size++;
                    }
                    if (f != -2){
                        sender_pid_list[f] = msg.sender_pid;
                        sender_msqid_list[f] = sender_msqid;
                        printf("msqid %d added\n", sender_msqid);
                    }
                }else{
                    for (int i = 0; i < nk_size; ++i){
                        if (sender_pid_list[i] == msg.sender_pid){ // found
                            sender_pid_list[i] = -1;
                            printf("msqid for pid %d were deleted\n", msg.sender_pid);
                            break;
                        }
                    }
                }
            }else{
                f = 1;
                for (int i = 0; i < nk_size; ++i){
                    if (sender_pid_list[i] == (int)msg.mtype){ // finds receiver's msqid
                        receiver_msqid = sender_msqid_list[i];                    
                        f = 0;
                        break;
                    }
                }
                if (f == 1){
                    printf("No user with pid = %ld!\n", msg.mtype);
                }else{
                    if (msgsnd(receiver_msqid, &msg, sizeof(msg)-sizeof(long), 0) == -1){
                        perror("msgsnd to client");
                    }
                }
            }
        }
    }
    
    msgctl(server_msqid, IPC_RMID, NULL);
    printf("Server is shutting down...\n");
    return 0; 
}
