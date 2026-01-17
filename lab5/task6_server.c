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

#define BUFFSIZE 1024
#define SERVER_KEY 12345   // fixed key for server

typedef struct msg_request {
    long mtype;           
    int client_msqid;      // msqid of clients queues (for response)
    char data[BUFFSIZE];   
} Request;

typedef struct msg_response {
    long mtype;            // pid of client
    char data[BUFFSIZE];
} Response;

int running = 1;

void handler(int sig){
     running = 0;
}

int main() {
    int server_msqid;
    Request req;
    Response resp;
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
        if (msgrcv(server_msqid, &req, sizeof(req) - sizeof(long), 0, 0) == -1) {
            if (running)
                perror("msgrcv");
            continue;
        }

        printf("\nGot request from client: %d\n", req.client_msqid);
        printf("    Data: %s\n", req.data);

        // Server's response
        resp.mtype = 1; 
        snprintf(resp.data, BUFFSIZE, 
                 "Response from PID = %d on request: \"%s\"", getpid(), req.data);

        if (msgsnd(req.client_msqid, &resp, strlen(resp.data) + 1, 0) == -1) {
            perror("msgsnd to client");
        } else {
            printf("    Response sent to msgid = %d\n", req.client_msqid);
        }

    }
    
    msgctl(server_msqid, IPC_RMID, NULL);
    printf("Server is shutting down...\n");
    return 0; 
}
