//
// Created by Bryan on 4/14/2021.
//

#ifndef ASSIGNMENT04_MAIN_H
#define ASSIGNMENT04_MAIN_H


//Shared memory structs

int sigusrHandler (int handlerID, int isType1);
int sigusrGenerator (int handlerID);
int signalReporter();

//Global protected counters for signals
extern struct protected_counter *recv_sigusr1, *recv_sigusr2;

struct SharedMem {
    int sig1_sent, sig2_sent;
    int sig1_recv, sig2_recv;
};

void handleSigusrMain (int sig);

#endif //ASSIGNMENT04_MAIN_H
