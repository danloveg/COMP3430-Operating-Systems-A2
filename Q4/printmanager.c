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


static pthread_cond_t notFull, notEmpty = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t bufferLock = PTHREAD_MUTEX_INITIALIZER;


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

    pthread_exit(NULL);
}


/**
 * Print server thread. Removes print jobs from the queue constantly.
 *
 * @param void *args: Not used.
 */
void *PrintServer(void *args) {
    long tid = pthread_self();
    int timeToPrint;
    int bufferSize = 32;
    time_t currentTime;
    struct tm * tmInfo;
    char time_buf[bufferSize];
    char * timeString = &time_buf[0];
    PrintRequest * currentJob = NULL;

    while (1) {
        // Get a job and assure it's valid
        removeFromBoundedBuffer(&currentJob);
        assert(currentJob != NULL && "Job cannot be NULL");
        assert(currentJob -> fileName != NULL && "Filename cannot be NULL");

        time(&currentTime);
        tmInfo = localtime(&currentTime);
        strftime(timeString, bufferSize, TIME_FORMAT, tmInfo);

        printf("Printer %ld STARTED print job:\n", tid);
        printf("\tTime:      %s\n", timeString);
        printf("\tClient ID: %ld\n", currentJob -> clientID);
        printf("\tFilename:  %s\n", currentJob -> fileName);
        printf("\tFile size: %d\n", currentJob -> fileSize);

        // Sleep to mimic printing
        timeToPrint = (currentJob -> fileSize) / BYTES_PER_SEC;
        sleep(timeToPrint);

        time(&currentTime);
        tmInfo = localtime(&currentTime);
        strftime(timeString, bufferSize, TIME_FORMAT, tmInfo);

        printf("Printer %ld FINISHED print job:\n", tid);
        printf("\tTime:      %s\n", timeString);
        printf("\tClient ID: %ld\n", currentJob -> clientID);
        printf("\tFilename:  %s\n", currentJob -> fileName);
        printf("\tFile size: %d\n", currentJob -> fileSize);
    }

    pthread_exit(NULL);
}


/**
 * Print client thread. Creates 6 print jobs
 *
 * @param void *args: Not used.
 */
void *PrintClient(void *args) {
    int i, filesize;
    long tid = pthread_self();
    unsigned int randseed = (unsigned int) tid;
    char file_buf[32];
    char *filename = &file_buf[0];
    char *filenameDynamic = NULL;
    PrintRequest request;

    for (i = 0; i < 6; i++) {
        // Filesize between 500 and 40000
        filesize = (rand_r(&randseed) % 39501) + 500;

        // Put the filename in the buffer, then duplicate it to a new string
        sprintf(filename, "File-%ld-%d", tid, i);
        filenameDynamic = strdup(filename);
        assert(filenameDynamic != NULL && "memory allocation failed in print client");

        // Create the request job
        request.clientID = tid;
        request.fileName = filenameDynamic;
        request.fileSize = filesize;

        insertIntoBoundedBuffer(&request);

        // Sleep for 0 to 3 seconds
        sleep(rand_r(&randseed) % 4);
    }

    pthread_exit(NULL);
}


/**
 * Insert a print job into the queue safely.
 * 
 * @param PrintRequest * req: The request to add. 
 */
void insertIntoBoundedBuffer(PrintRequest * req) {
    pthread_mutex_lock(&bufferLock);
    // If the queue is full, wait until notFull
    while (queuefull() == true)
        pthread_cond_wait(&notFull, &bufferLock);
    // Insert an item and signal notEmpty
    bool entered = enter(req);
    assert(entered == true && "Entry into queue failed");
    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&bufferLock);
}


/**
 * Remove a print job from the queue safely.
 * 
 * @param PrintRequest ** req: Gets assigned to the print job received.
 */
void removeFromBoundedBuffer(PrintRequest ** req) {
    pthread_mutex_lock(&bufferLock);
    // If the queue is empty, wait until notEmpty
    while (queueempty() == true)
        pthread_cond_wait(&notEmpty, &bufferLock);
    // Remove an item and signal notFull
    bool left = leave(req);
    assert(left == true && "Removal from queue failed");
    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&bufferLock);
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
