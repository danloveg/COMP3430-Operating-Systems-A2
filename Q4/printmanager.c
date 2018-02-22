// *****************************************************************************
// printmanager.c
//
// Purpose: Simulate a multi-threaded print manager with multiple clients and a
//     finite number of printers.
// Author: Daniel Lovegrove
// Version: Feb 22/2018
// *****************************************************************************

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "printmanager.h"
#include "printqueue.h"


int main(int argc, char *argv[]) {
    int NumPrintClients, NumPrinters, i = 0;

    getArgs(argc, argv, &NumPrintClients, &NumPrinters);
    printf("Number of clients: %d\nNumber of printers: %d\n", NumPrintClients, NumPrinters);

    createQueue(QUEUE_LEN);

    // Create clients
    pthread_t clientThreads[NumPrintClients];
    for (i = 0; i < NumPrintClients; i++) {
        pthread_create(&clientThreads[i], NULL, PrintClient, NULL);
    }

    // Create servers
    pthread_t serverThreads[NumPrinters];
    for (i = 0; i < NumPrinters; i++) {
        pthread_create(&serverThreads[i], NULL, PrintServer, NULL);
    }

    destroyQueue();
    pthread_exit(NULL);
}


/**
 * Print server thread.
 */
void *PrintServer(void *args) {
    // long tid = pthread_self();
    return NULL;
}


/**
 * Print client thread.
 */
void *PrintClient(void *args) {
    int i, filesize;
    long tid = pthread_self();
    unsigned int randseed = (unsigned int) tid;
    char file_buf[32];
    char *filename = &file_buf[0];
    char *filenameDynamic = NULL;
    PrintRequest * request;

    for (i = 0; i < 6; i++) {
        // Filesize between 500 and 40000
        filesize = (rand_r(&randseed) % 39501) + 500;
        sprintf(filename, "File-%ld-%d", tid, i);
        filenameDynamic = strdup(filename);

        request = calloc(1, sizeof(PrintRequest));
        request -> clientID = tid;
        request -> fileName = filenameDynamic;
        request -> fileSize = filesize;

        insertIntoBoundBuffer(request);

        // Sleep for 0 to 3 seconds
        sleep(rand_r(&randseed) % 4);
    }

    return NULL;
}


void insertIntoBoundBuffer(PrintRequest * req) {

}

void removeFromBoundedBuffer(PrintRequest * req) {

}


/**
 * Get NumPrintClients and NumPrinters from command line args.
 *
 * @param int argc: length of argv
 * @param char *argv[]: command line arguments
 * @param int *numprintclients: gets assigned the number of threads
 * @param int *numprinters: gets assigned the number of elements
 */
void getArgs(int argc, char *argv[], int *numprintclients, int *numprinters) {
    char *eptr;

    assert(argc == 3 && "Only two arguments are allowed");
    *numprintclients  = strtol(argv[1], &eptr, 10);
    assert(*numprintclients != 0 && eptr != argv[1] && "Error converting first arg to int");
    *numprinters = strtol(argv[2], &eptr, 10);
    assert(*numprinters != 0 && eptr != argv[2] && "Error converting second arg to int");
}
