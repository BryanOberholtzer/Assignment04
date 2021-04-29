


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#include <signal.h>

#include "main.h"

#define NUM_SIG_HANDLERS 4
#define NUM_SIG_GENERATORS 3
#define DEBUG_NUM_EXECS 8

#define SIGUSR1_MSG "This is the sigint1 message."

int main(int argc, char** argv) {

    //Create shared memory region
    int shmID = shmget(IPC_PRIVATE, sizeof(struct SharedMem), IPC_CREAT | 0666);
    if(shmID < 0) {
        perror("Failure creating shared memory with shmget");
        exit(1);
    }

    struct SharedMem *sm;
    sm = (struct SharedMem *) shmat(shmID, NULL, 0);

    //Fork into processes
    int pid[8];

    for(int i = 0; i < DEBUG_NUM_EXECS; i++) {
        if ((pid[i] = fork()) == -1) {                 //Fork process
            perror("Fork failed to execute");
            exit(1);
        }
        else if(pid[i] == 0) {
            if(i <= 3) {                            //Signal handler processes
                if(i <= 1) {    //i = 0 or 1
                    sigusrHandler(i, 1);    //SIGUSR1 handler
                }
                else {          //i = 2 or 3
                    sigusrHandler(i, 0);    //SIGUSR2 handler
                }
                exit(0);
            }
            else if (i <= 6) {                      //Signal generating processes
                //sigusrGenerator(i);
                exit(0);
            }
            else {                                  //Single Reporter process
                //signalReporter();
                exit(0);
            }
        }
        printf("%d\n", i);
    }

    sleep(1);

    signal(SIGUSR1, handleSigusrMain);
    signal(SIGUSR2, handleSigusrMain);

    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) kill(0, SIGUSR1);
        else kill(0, SIGUSR2);
        sleep(1);
    }

    kill(0, SIGINT);

    int status;
    wait(&status);

    return 0;
}

void handleSigusrMain (int sig) {
    if (sig == SIGUSR1) {
        puts("main receives SIGURS1");
    }
    else if (sig == SIGUSR2) {
        puts("main receives SIGUSR2");
    }
}