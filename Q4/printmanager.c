// *****************************************************************************
// printmanager.c
//
// Purpose: Simulate a multi-threaded print manager with multiple clients and a
//     finite number of printers.
// Author: Daniel Lovegrove
// Version: Feb 20/2018
// *****************************************************************************

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "printmanager.h"
#include "printqueue.h"


void getArgs(int argc, char *argv[], int *numprintclients, int *numprinters);


int main(int argc, char *argv[]) {
    int NumPrintClients, NumPrinters = 0;
    getArgs(argc, argv, &NumPrintClients, &NumPrinters);

    printf("Number of clients: %d\nNumber of printers: %d\n", NumPrintClients, NumPrinters);
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
