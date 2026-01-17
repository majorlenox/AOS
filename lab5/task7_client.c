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
#define SERVER_KEY 12345
#define NAME_LENGTH 32

typedef struct {
    long mtype; // PID of receiver or 1 -> broadcast
    int  sender_pid;
    char name[NAME_LENGTH];
    char data[BUFFSIZE];
} Message;

int running = 1;

void handler(int sig){
     running = 0;
}

int main() {
    struct sigaction sa;

    // catch ctrl+C to shutdown
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    key_t client_key;
    int client_msqid, server_msqid;
   
    // unique queue for responses
    client_key = ftok("task7_client.c", getpid());
    client_msqid = msgget(client_key, IPC_CREAT | 0666);
      
    server_msqid = msgget(SERVER_KEY, 0666);
    if (server_msqid == -1) {
        perror("msgget server queue");
        printf("Start a server by: ./task7_server\n");
        msgctl(client_msqid, IPC_RMID, NULL); // delete queue
        exit(1);
    }

    printf("Client PID = %d has created a queue msqid = %d\n", getpid(), client_msqid);

    // send gratuitous to server
    Message msg;
    msg.mtype = msg.sender_pid = getpid();
    sprintf(msg.data ,"%d", client_msqid);
    if (msgsnd(server_msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
        perror("msgsnd can't send gratuitous msg to server");
    
    char name[NAME_LENGTH];
    printf("Your PID is %d\n", getpid());
    printf("Enter your name: ");
    fgets(name, NAME_LENGTH-1, stdin);
    name[strlen(name)-1] = '\0';

    msg.sender_pid = getpid();
    memcpy(msg.name, name, NAME_LENGTH);

    pid_t pid = fork(); // for reading

    if (pid == 0){
        // Child is reading
        while(running){
            Message rec;
            int n = msgrcv(client_msqid, &rec, sizeof(rec)-sizeof(long), 0, 0);
            if (n == -1) {
                perror("msgrcv");
                break;
            }
            
           printf("\nMessage from: %s (pid = %d)\n Text: %s\n", rec.name, rec.sender_pid, rec.data);
           fflush(stdout);
           printf("Enter new message: ");
           fflush(stdout);
        }
        exit(0);
    }    
    // Parent -> enter and send message
    printf("Message format: \"@PID_of_receiver text of message\"\n");
    printf("Enter message: ");
    fflush(stdout);

    char buffer[BUFFSIZE];
    while (running && fgets(buffer, BUFFSIZE-1, stdin)) {
        buffer[strlen(buffer) - 1]= '\0';
        memcpy(msg.data, buffer, strlen(buffer) + 1);
        // Parse for receiver's pid (target_pid)
        if (buffer[0] == '@') {
            char *space = strchr(buffer, ' '); // find pointer to first space symbol
            if (space) {
                int target_pid;
                if (sscanf(buffer + 1, "%d", &target_pid) == 1) {
                    msg.mtype = target_pid;
                    memcpy(msg.data, space + 1, strlen(space + 1) + 1); // Copy text
                } else {
                    msg.mtype = 1;  // can't recognize pid -> broadcast
                }
            } else {
                msg.mtype = 1;
            }
        } else {
            msg.mtype = 1;  // broadcast
        }

        // Sending to server
        if (strlen(msg.data) != 0 && msgsnd(server_msqid, &msg, sizeof(msg)-sizeof(long), 0) == -1) {
            perror("msgsnd");
        }

        printf("Enter new message: ");
        fflush(stdout);
    }

    // delete msqid on server
    msg.mtype = msg.sender_pid = getpid();
    memcpy(msg.data, "-1", 3); // -1 -> delete msqid
    if (msgsnd(server_msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
        perror("msgsnd can't delete client's msqid");

    kill(pid, SIGTERM); // kill reader (child)
    waitpid(pid, NULL, 0);
    msgctl(client_msqid, IPC_RMID, NULL);
    printf("Client PID = %d is shutting down...\n", getpid());
    return 0;
}
