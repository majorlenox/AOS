#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h> // ctime
#include<errno.h>

#define BUFFSIZE 1024

/*
2. Написать программу, позволяющую читать сообщения из очереди и выводить их на экран.
Идентификатор очереди и тип сообщения передается через аргументы командной строки.
*/

typedef struct our_msgbuf {
    long mtype;
    char buffer[BUFFSIZE];
} Message;

int main(int argc, char* argv[]){
    int msgid;
    long int mtype;
      
    if (argc != 3){
        printf("usage: ./task2 <msgid> <type>\n");
        exit(0);
    }

    msgid = atoi(argv[1]);
    mtype = atoi(argv[2]);

    Message msg;

    msgrcv(msgid, &msg, BUFFSIZE, mtype, IPC_NOWAIT);
    if (errno != ENOMSG) {
        printf("Received message with mtype = %ld, data = \"%s\"\n", msg.mtype, msg.buffer);
    }else{
        printf("No messages with type = %ld in queue\n", mtype);
    }
  
    return 0;
}
