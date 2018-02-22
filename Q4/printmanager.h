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
