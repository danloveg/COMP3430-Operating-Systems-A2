// *****************************************************************************
// printmanager.h
//
// Author: Daniel Lovegrove
// Version: Feb 21/2018
// *****************************************************************************

#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#define QUEUE_LEN 4

typedef struct __PRINT_REQUEST {
    long clientID;
    char *fileName;
    int fileSize;
} PrintRequest;

#endif
