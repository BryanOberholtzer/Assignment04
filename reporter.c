//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int signalReporter () {
    printf("%s\n", "This is the reporter");

    struct timeval startTime, currentTime;
    gettimeofday(&startTime, NULL);

    double elapsedTime = 0;

    while(elapsedTime <= 10) {
        gettimeofday(&currentTime, NULL);

        elapsedTime =
                (double) (currentTime.tv_sec - startTime.tv_sec) +
                ((double) (currentTime.tv_usec - startTime.tv_usec) / 1000000);

        printf("%lf\n", elapsedTime);
        usleep(300000);
    }

    return 0;
}