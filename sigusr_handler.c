//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "main.h"
#include "protected_counter.h"

void handleSigusrHandler (int type);

int sigusrHandler (int handlerID, int isType1) {
    printf("This is handler #%d\n", handlerID);

    //Determine SIGUSR to respond to and to ignore depending on type
    int signal_respond, signal_ignore;

    signal_respond = (isType1)? SIGUSR1 : SIGUSR2;
    signal_ignore = (isType1) ? SIGUSR2 : SIGUSR1;

    //Set signal responses
    if(signal(signal_respond, handleSigusrHandler) == SIG_ERR) {
        perror("Error calling signal");
        exit(1);
    }
    if(signal(signal_ignore, SIG_IGN) == SIG_ERR) {
        perror("Error calling signal");
        exit(1);
    }

    while(1) {
        pause();
    }

    return 0;
}

void handleSigusrHandler (int type) {
    if (type == SIGUSR1) {
        inc(recv_sigusr1);
        //printf("sigusr1: %d\n", get_value(recv_sigusr1));
    }
    else if (type == SIGUSR2) {
        inc(recv_sigusr2);
        //printf("sigusr2: %d\n", get_value(recv_sigusr2));
    }
    else {
        puts("unknown signal");
    }
}