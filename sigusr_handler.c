//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>

int sigusrHandler (int handlerID) {
    printf("This is handler #%d\n", handlerID);
    return 0;
}