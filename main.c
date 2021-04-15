


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/time.h>

#include "main.h"

#define NUM_SIG_HANDLERS 4
#define NUM_SIG_GENERATORS 3

int main(int argc, char** argv) {

    //Create shared memory region
    int shmID = shmget(IPC_PRIVATE, sizeof(struct SharedMem), IPC_CREAT | 0666);
    if(shmID < 0) {
        perror("Failure creating shared memory with shmget");
        exit(1);
    }

    struct SharedMem *sm;
    sm = (struct SharedMem *) shmat(shmID, NULL, 0);

    //Overwrite SIGUSR1 and SIGUSR2

    //Fork into processes
    int pid;

    for(int i = 0; i < 8; i++) {

        if ((pid = fork()) == -1) {                 //Fork process
            perror("Fork failed to execute");
            exit(1);
        }
        else if(pid == 0) {
            if(i <= 3) {                            //Signal handler processes
                sigusrHandler(i);
                exit(0);
            }
            else if (i <= 6) {                      //Signal generating processes
                sigusrGenerator(i);
                exit(0);
            }
            else {                                  //Single Reporter process
                signalReporter();
                exit(0);
            }
        }
        printf("%d\n", i);
    }

    puts("I am the parent");

    return 0;
}
