// *****************************************************************************
// printmanager.h
//
// Author: Daniel Lovegrove
// Version: Feb 22/2018
// *****************************************************************************

#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#define QUEUE_LEN 4
#define BYTES_PER_SEC 4000
#define BUF_SIZE 32
#define NUM_ITERATIONS 6
#define FILE_SIZE_MAX 40000
#define FILE_SIZE_MIN 500
#define SLEEP_TIME_MAX 3
#define TIME_FORMAT "%Y/%m/%d %H:%M:%S"

typedef struct __PRINT_REQUEST {
    long clientID;
    char *fileName;
    int fileSize;
} PrintRequest;

void *PrintServer(void *args);
void *PrintClient(void *args);
void insertIntoBoundedBuffer(PrintRequest * req);
void removeFromBoundedBuffer(PrintRequest ** req);
void getArgs(int argc, char *argv[], int *numprintclients, int *numprinters);

#endif
