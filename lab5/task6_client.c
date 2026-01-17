#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>

#define BUFFSIZE 1024
#define SERVER_KEY 12345

typedef struct msg_request {
    long mtype;
    int client_msqid;
    char data[BUFFSIZE];
} Request;

typedef struct msg_response {
    long mtype;
    char data[BUFFSIZE];
} Response;

int main() {
    key_t client_key;
    int client_msqid, server_msqid;
    Request req;
    Response resp;

    // unique queue for responses
    client_key = ftok("task6_client.c", getpid());
    client_msqid = msgget(client_key, IPC_CREAT | 0666);
      
    server_msqid = msgget(SERVER_KEY, 0666);
    if (server_msqid == -1) {
        perror("msgget server queue");
        printf("Start server by: ./task6_server\n");
        msgctl(client_msqid, IPC_RMID, NULL); // delete queue
        exit(1);
    }

    printf("Client PID = %d has created a queue msqid = %d\n", getpid(), client_msqid);
    
    while(1){
        // Request
        req.mtype = 1; 
        req.client_msqid = client_msqid;
        printf("Enter message for server:\n");
        if(fgets(req.data, BUFFSIZE-1, stdin) == NULL) // Ctrl+D -> shutting down
            break;
        req.data[BUFFSIZE-1]='\0'; //   get rid of \n

        msgsnd(server_msqid, &req, sizeof(req) - sizeof(long), 0);
        printf("Request sent\n");

        // Wait for response
        msgrcv(client_msqid, &resp, sizeof(resp) - sizeof(long), 1, 0);
        printf("\nServer's response:\n%s\n", resp.data);
        if (strcmp("exit", resp.data) == 0)
            break;
    }

    msgctl(client_msqid, IPC_RMID, NULL);
    printf("Client PID = %d is shutting down...\n", getpid());
    return 0;
}
