#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<time.h> // ctime
#include<errno.h>

/*
    5. Написать программу, позволяющую удалять очереди сообщений по идентификатору.
       Обработать возможные ошибочные ситуации.
*/

// Create queue: ipcmk --queue -p 0666
// change user: su john, john's password=123, (/etc/passwd)

int main(int argc, char* argv[]){
    int msgid;
      
    if (argc != 2){
        printf("usage: ./task5 <msgid>\n");
        exit(0);
    }

    msgid = atoi(argv[1]);
    
    msgctl(msgid, IPC_RMID, "Hello");
    
    perror("1");
    switch(errno){
        case EACCES:
            // never occurs, cuz deleting only for owners of queue
            // it can occur, when you are trying to change attributes of queue etc.
        break;
        case EFAULT:
            // never occurs, cuz IPC_RMID ignores 3rd argument
        break;
        case EIDRM:
            printf("ERROR: Can't delete queue %d, its already deleted\n", msgid);
        break;
        case EINVAL:
            printf("ERROR: Can't delete queue %d, queue doesn't exists\n", msgid);
        break;
        case EPERM:
            printf("ERROR: Can't delete queue %d, no permissions to do that\n", msgid);
        break;
        default:   
            printf("Queue %d were deleted\n", msgid);
    }
  
    return 0;
}
