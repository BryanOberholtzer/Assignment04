//
// Created by Bryan on 4/14/2021.
//

#ifndef ASSIGNMENT04_MAIN_H
#define ASSIGNMENT04_MAIN_H

//Shared memory structs

int sigusrHandler (int handlerID, int isType1);
int sigusrGenerator (int handlerID);
int signalReporter();

struct SharedMem {
    int sig1_sent, sig2_sent;
    int sig1_recv, sig2_recv;
};

void handleSigusrMain (int sig);

#endif //ASSIGNMENT04_MAIN_H
