


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
#include "protected_counter.h"

#define NUM_SIG_HANDLERS 4
#define NUM_SIG_GENERATORS 3
#define DEBUG_NUM_EXECS 8

struct protected_counter *recv_sigusr1, *recv_sigusr2;

int main(int argc, char** argv) {

    /*
    //Create shared memory region
    int shmID = shmget(IPC_PRIVATE, sizeof(struct SharedMem), IPC_CREAT | 0666);
    if(shmID < 0) {
        perror("Failure creating shared memory with shmget");
        exit(1);
    }

    struct SharedMem *sm;
    sm = (struct SharedMem *) shmat(shmID, NULL, 0);
    */

    //Initialize SIGUSR1/2 counters
    if ((recv_sigusr1 = make_counter("SIGUSR1_Recv")) == NULL) {
        fprintf(stderr, "Error creating shared counter");
        exit(1);
    }
    if ((recv_sigusr2 = make_counter("SIGUSR2_Recv")) == NULL) {
        fprintf(stderr, "Error creating shared counter");
        exit(1);
    }
    //Set both counters to 0
    set(recv_sigusr1, 0);
    set(recv_sigusr2, 0);

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
    }

    sleep(1);       //Let all child processes set-up

    //Set SIGUSR1/2 responses for main
    if (signal(SIGUSR1, handleSigusrMain) == SIG_ERR) {
        perror("Failed to call signal");
        exit(1);
    }
    if (signal(SIGUSR2, handleSigusrMain) == SIG_ERR) {
        perror("Failed to call signal");
        exit(1);
    }

    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) kill(0, SIGUSR1);
        else kill(0, SIGUSR2);
        sleep(1);
    }

    kill(0, SIGINT);

    int status;
    wait(&status);

    //Cleanup counters
    cleanup(recv_sigusr1);
    cleanup(recv_sigusr2);

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