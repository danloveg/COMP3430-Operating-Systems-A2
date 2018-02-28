# COMP 3430 Winter 2018 Assignment 2 - Synchronization and Threads in C

## Overview
Assignment 2 involves synchronization using semaphores, locks, and condition variables. Two of the questions (Q2 & Q4) are written in code (all in C). Question 2 is a simple demonstration that pthread_create() in C does not create kernel level threads, but creates user level threads. Question 4 is a simulation of multiple clients adding print jobs to a finite print queue with printers "printing" out the files. The issue question 4 poses is that of how to safely use a bounded buffer. I use a "monitor" to guard the entering and exiting of the queue.

## Details
All of the code has only been tested on Linux (Scientific and Ubuntu) and cannot be guaranteed to work on Mac or Windows. The assignment questions that are not in code are in the PDF file at the root of this repository.

## Running the Code

### Question 2
1. From a terminal, change directory to Q2:  `cd Q2`
2. Make the executable:  `make`
3. Run the executable:  `./question2`

Q2 creates output to a file called q2output.txt in the Q2 folder. This text file demonstrates that there are no kernel level threads created in the process. See the assignment PDF for more info. The program will run until you Ctrl-C it.

### Question 4
1. From a terminal, change directory to Q4:  `cd Q4`
2. Make the executable:  `make`
3. Run the executable with two integer arguments:  `./printmanager 5 2`

The arguments for printmanager correspond to the number of print clients and the number of print servers, respectively. The code uses a bounded buffer to demonstrate many clients queueing up print jobs and a finite number of slow printers 'printing' the queued jobs. There is no actual printing done, the 'printing' is simulated by delaying an amount of time proportional to the file size of the queued job.
