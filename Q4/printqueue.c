// *****************************************************************************
// printqueue.c
//
// Purpose: Print request queue
// Author: Daniel Lovegrove
// Version: Feb 21/2018
// *****************************************************************************

#include "printqueue.h"
#include <assert.h>
#include <stddef.h>

typedef struct __PRINT_JOB_QUEUE {
    int currLen;
    int currIndex;
    int maxLen;
    PrintRequest * queueArray;
} PrintJobQueue;

PrintJobQueue * queue = NULL;

void createQueue(int len) {
    assert(len > 0 && "Queue length must be greater than zero");
    queue -> queueArray = malloc(sizeof(PrintRequest) * 4);
    assert(queue != NULL && "Memory allocation failed in queue creation");
    queue -> maxLen = len;
    queue -> currLen = 0;
    queue -> currIndex = 0;
}

void destroyQueue() {
    free (queue -> queueArray);
    queue = NULL;
}

bool enter(PrintRequest * req) {
    assert(queue != NULL && "Queue must be initialized");
    bool entered = false;

    if (full() == false) {
        queue -> queueArray[(queue -> currIndex + queue -> currLen) % queue -> maxLen] = *req;
        queue -> currLen++;
        entered = true;
    }

    return entered;
}

bool exit(PrintRequest * req) {
    assert(queue != NULL && "Queue must be initialized");
    bool reqReturned = false;

    if (empty() == false) {
        req = &(queue -> queueArray[queue -> currIndex]);
        queue -> currIndex = (queue -> currIndex + 1) % queue -> maxLen;
        queue -> currLen--;
    }

    return reqReturned;
}

bool full() {
    assert(queue != NULL && "Queue must be initialized");
    bool full = false;

    if ((queue -> currLen) == (queue -> maxLen)) {
        full = true;
    } else {
        full = false;
    }

    return full;
}

bool empty() {
    assert(queue != NULL && "Queue must be initialized");
    bool empty = false;

    if ((queue -> currLen) == 0) {
        empty = true;
    } else {
        empty = false;
    }

    return empty;
}
