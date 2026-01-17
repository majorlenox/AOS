#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h> // ctime
#include<errno.h>

#define BUFFSIZE 5

/*
    4. Модифицировать программу п. 2, позволив ей принимать 
    первые несколько байтов сообщений произвольных размеров.
*/

typedef struct our_msgbuf {
    long mtype;
    char buffer[BUFFSIZE];
} Message;

int main(int argc, char* argv[]){
    int msgid;
    long int mtype;
      
    if (argc != 3){
        printf("usage: ./task4 <msgid> <type>\n");
        exit(0);
    }

    msgid = atoi(argv[1]);
    mtype = atoi(argv[2]);
    
    Message msg;

    msgrcv(msgid, &msg, BUFFSIZE-1, mtype, IPC_NOWAIT | MSG_NOERROR);
    msg.buffer[BUFFSIZE-1] = '\0';

    if (errno != ENOMSG) {
        printf("Received message with mtype = %ld, data = \"%s\"\n", mtype, msg.buffer);
    }else{
        printf("No messages with type = %ld in queue\n", mtype);
    }
  
    return 0;
}
