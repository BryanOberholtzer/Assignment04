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

    //Determine SIGUSR to respond to and to ignore depending on type
    int signal_respond, signal_ignore;
    if (isType1) {
        signal_respond = SIGUSR1;
        signal_ignore = SIGUSR2;
    }
    else {
        signal_respond = SIGUSR2;
        signal_ignore = SIGUSR1;
    }

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
        puts("sigusr1");
    }
    else if (type == SIGUSR2) {
        puts("sigusr2");
    }
    else {
        puts("unknown signal");
    }
}