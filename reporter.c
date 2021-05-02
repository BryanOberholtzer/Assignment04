//
// Created by Bryan on 4/14/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "main.h"
#include "protected_counter.h"


int num_sig_recv;               //# of signals received since the last 10
double avg_interval_buf[10];           //Buffer for time between signal receptions
struct timeval prev_time_recv, current_time_recv;

void handleSigusrReporter(int sig);

int signalReporter () {
    num_sig_recv = 0;
    memset(avg_interval_buf, 0, sizeof(avg_interval_buf));


    printf("%s\n", "This is the reporter");

    //double elapsedTime = 0;

    //Set signal responses
    if(signal(SIGUSR1, handleSigusrReporter) == SIG_ERR) {
        perror("Error calling signal");
        exit(1);
    }
    if(signal(SIGUSR2, handleSigusrReporter) == SIG_ERR) {
        perror("Error calling signal");
        exit(1);
    }

    gettimeofday(&prev_time_recv, NULL);        //Set to start time

    while(1) {
        pause();
    }

    /*
    struct timeval startTime, currentTime;
    gettimeofday(&startTime, NULL);

    while(elapsedTime <= 10) {
        gettimeofday(&currentTime, NULL);

        elapsedTime =
                (double) (currentTime.tv_sec - startTime.tv_sec) +
                ((double) (currentTime.tv_usec - startTime.tv_usec) / 1000000);

        printf("%lf\n", elapsedTime);
        usleep(300000);
    }
    */

    return 0;
}

void handleSigusrReporter(int sig) {

    //save system time
    gettimeofday(&current_time_recv, NULL);

    //Calculate elapsed time since last signal
    double current_elapsed =
            (double) (current_time_recv.tv_sec - prev_time_recv.tv_sec) +
            ((double)(current_time_recv.tv_usec - prev_time_recv.tv_usec) / 1000000);

    //printf("elapsed: %lf\nnum_sigs: %d\nmod 10: %d\n", current_elapsed, num_sig_recv, num_sig_recv % 10);

    //Increment num_sig_recv
    num_sig_recv++;

    //Add elapsed time to buffer, positioned % 10
    avg_interval_buf[(num_sig_recv % 10)] = current_elapsed;

    //Update prev_time_recv
    prev_time_recv = current_time_recv;

    double average = 0;
    if (num_sig_recv % 10 == 0) {
        //Calculate average time between signals received
        for (int i = 0; i < 10; i++) {
            average += avg_interval_buf[i];
        }
        average = average / 10;

        struct timeval temp_time;
        gettimeofday(&temp_time, NULL);
        printf("System time: %lf\n", (double)(temp_time.tv_sec + ((double) temp_time.tv_usec / 1000000)));
        printf("SIGUSR1 Sent: %d\nSIGUSR2 Sent: %d\nSIGUSR1 Received: %d\nSIGUSR2 Received: %d\n",
               get_value(sent_sigusr1), get_value(sent_sigusr2), get_value(recv_sigusr1), get_value(recv_sigusr2));
        printf("Avg. interval: %lf\n", average);
    }


    //Print sig1/2sent, sig1/2recv, avg time


}