//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handleSigusrHandler (int type);

int sigusrHandler (int handlerID, int isType1) {
    printf("This is handler #%d\n", handlerID);

    //Set behavior for SIGUSR1 and SIGUSR2 signals
    if (isType1) {      //Signal handler for SIGUSR1
        //Respond to SIGUSR1
        //Ignore SIGUSR2
        if(signal(SIGUSR1, handleSigusrHandler) == SIG_ERR) {
            perror("Error calling signal");
            exit(1);
        }
        if(signal(SIGUSR2, SIG_IGN) == SIG_ERR) {
            perror("Error calling signal");
            exit(1);
        }
    }
    else {              //Signal handler for SIGUSR2
        //Respond to SIGUSR2
        //Ignore SIGUSR1
        if(signal(SIGUSR2, handleSigusrHandler) == SIG_ERR) {
            perror("Error calling signal");
            exit(1);
        }
        if(signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
            perror("Error calling signal");
            exit(1);
        }
    }



    while(1) {
        pause();
    }

    return 0;
}

void handleSigusrHandler (int type) {
    if (type == SIGUSR1) {
        puts("sigusr1");
    }
    else if (type == SIGUSR2) {
        puts("sigusr2");
    }
    else {
        puts("unknown signal");
    }
}