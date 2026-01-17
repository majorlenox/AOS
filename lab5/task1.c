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
1. Создать очередь сообщений или получить доступ к уже существующей очереди.
   Вывести на экран статистическую информацию об очереди сообщений.
   Поместить несколько сообщений различных типов в очередь.
*/

// ipcs             - show queues
// ipcrm -q <msgid> - delete queue with msgid

typedef struct our_msgbuf {
    long mtype;
    char buffer[BUFFSIZE];
} Message;

int main(int argc, char* argv[]){
    key_t key;
    int msgid;
    long int a;
    struct msqid_ds ds;
    
    // Creation
    key = ftok("task1.c", 1);
    msgid = msgget(key, IPC_CREAT | 0666);
    
    if (argc == 1)
        printf("To send or receive message usage: ./task1 [1|2]\n");

    // Stat_info
    msgctl(msgid, IPC_STAT, &ds);
    printf("msqid_ds for id = %d:\n", msgid);
    printf("    msg_perm:\n"
           "        __key = %d\n"
           "        uid = %d\n"
           "        gid = %d\n"
           "        cuid = %d\n"
           "        cgid = %d\n"
           "        mode = 0%o\n"
           "        __seq = %d\n",
          ds.msg_perm.__key, ds.msg_perm.uid, ds.msg_perm.gid, ds.msg_perm.cuid,
          ds.msg_perm.cgid, ds.msg_perm.mode, ds.msg_perm.__seq);
    printf("msg_stime = %s" // inits when sending/receving
           "msg_rtime = %s" 
           "msg_ctime = %s"
           "msg_cbytes = %lu\n"
           "msg_qnum = %lu\n"
           "msg_qbytes = %lu\n"
           "msg_lspid = %d\n"
           "msg_lrpid = %d\n",
          ctime(&ds.msg_stime), ctime(&ds.msg_rtime), ctime(&ds.msg_ctime),
          ds.msg_cbytes, ds.msg_qnum, ds.msg_qbytes, ds.msg_lspid, ds.msg_lrpid);

    Message msg;
    if (argc != 1){
        if(argv[1][0] == '1'){
        // Sending
        printf("Enter message type (integer)\n");
        scanf("%ld", &msg.mtype);
        printf("Enter message\n");
        scanf("%*c");
        fgets(msg.buffer, BUFFSIZE-1, stdin);
        msg.buffer[strlen(msg.buffer)-1]='\0'; // no \n
        msgsnd(msgid, &msg, strlen(msg.buffer) + 1, 0);
        printf("Message sent\n");
        }else{
            if (argv[1][0] == '2'){
                // Receiving
                printf("Enter message type\n");
                scanf("%ld", &a);
                msgrcv(msgid, &msg, BUFFSIZE, a, IPC_NOWAIT); // read
                if (errno != ENOMSG) {
                printf("Received message with mtype = %ld, data = \"%s\"\n", msg.mtype, msg.buffer);
                }else{
                    printf("No messages with type = %ld in queue\n", a);
                }
            }
        }
    }

    return 0;
}
