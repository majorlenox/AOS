#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY 12345

typedef struct {
    long mtype;
    int counter;
} PingPong;

int main(int argc, char* argv[]) {
    int msqid = msgget(MSG_KEY, IPC_CREAT | 0666);
    int max_count = 20;        
    if (argc != 1)
        sscanf(argv[1], "%d", &max_count);

    pid_t pid = fork();
    
    if (pid == 0) {
        // player 2
        PingPong ball;
        while (1) {
            msgrcv(msqid, &ball, sizeof(ball) - sizeof(long), 1, 0);
            
            if (ball.counter == -1) break; // player 2 lost

            if (ball.counter >= max_count) {
                printf("Player 2: Game over\n");
                ball.mtype = 2;
                ball.counter = -1;
                msgsnd(msqid, &ball, sizeof(ball)-sizeof(long), 0);
                break;
            }
            
            printf("Player 2: Got ball, counter = %d\n", ball.counter);
            usleep(10*1000);
            ball.mtype = 2;
            ball.counter++;
            msgsnd(msqid, &ball, sizeof(ball) - sizeof(long), 0);
        }
    } else {
        // player 1 (starting game)
        PingPong ball;
        ball.mtype = 1;
        ball.counter = 1;
        
        printf("Player 1: Starting game\n");
        msgsnd(msqid, &ball, sizeof(ball) - sizeof(long), 0);
        
        while (1) {
            msgrcv(msqid, &ball, sizeof(ball) - sizeof(long), 2, 0);
            
            if (ball.counter == -1) break; // player 1 lost

            if (ball.counter >= max_count) {
                printf("Player 1: Game over\n");
                ball.mtype = 1;
                ball.counter = -1;
                msgsnd(msqid, &ball, sizeof(ball)-sizeof(long), 0);
                break;
            }
            
            printf("Player 1: Got ball back, counter = %d\n", ball.counter);
            
            usleep(10*1000);

            ball.mtype = 1;
            ball.counter++;
            msgsnd(msqid, &ball, sizeof(ball) - sizeof(long), 0);
        }
        
        waitpid(pid, NULL, 0);
        msgctl(msqid, IPC_RMID, NULL);
    }
    
    return 0;
}
