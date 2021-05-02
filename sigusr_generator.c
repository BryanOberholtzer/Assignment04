//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <signal.h>

#include "main.h"
#include "protected_counter.h"

int randInt (int min, int max);

int sigusrGenerator (int handlerID) {

    //Set default SIGUSR1/2 responses for main
    if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
        perror("Failed to call signal");
        exit(1);
    }
    if (signal(SIGUSR2, SIG_IGN) == SIG_ERR) {
        perror("Failed to call signal");
        exit(1);
    }

    printf("%s\n", "This is the generator");

    srand(getpid());

    sleep(1);

    int is_sigusr1;
    int rand_wait;
    int signal_type;
    while(1) {
        is_sigusr1 = rand() & 1;          //Generate random 0 or 1 to determine SIGUSR1/2
        signal_type = is_sigusr1 ? SIGUSR1 : SIGUSR2;
        struct protected_counter *to_inc = is_sigusr1 ? sent_sigusr1 : sent_sigusr2;

        kill(0, signal_type);       //Signal respectice SIGUSR1/2

        inc(to_inc);        //Increment respective counter

        //printf("counter: %d\n", get_value(to_inc));

        rand_wait = randInt(10000, 100000);     //Pick between .1 and .01 seconds to wait
        usleep(rand_wait);
    }
}

int randInt (int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
