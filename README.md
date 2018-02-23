# COMP 3430 Winter 2018 Assignment 2 - Synchronization and Threads


Synchronization and Threads in C.

Two of the assignment questions are code, the rest are answered in the assignment PDF. All of the code has only been tested on Linux (Scientific and Ubuntu) and cannot be guaranteed to work on Mac or Windows.

To run the Q2 code:

1. Change directory to Q2:  `cd Q2`
2. Make the executable:  `make`
3. Run the executable:  `./question2`

Q2 creates output to a file called q2output.txt in the Q2 folder. This text file demonstrates that there are no kernel level threads created in the process. See the assignment PDF for more info.

To run the Q4 code:

1. Change directory to Q4:  `cd Q4`
2. Make the executable:  `make`
3. Run the executable with two integer arguments:  `./printmanager 5 2`

The arguments for printmanager correspond to the number of print clients and the number of print servers, respectively. The code uses a bounded buffer to demonstrate many clients queueing up print jobs and a finite number of slow printers 'printing' the queued jobs. There is no actual printing done, the 'printing' is simulated by delaying an amount of time proportional to the file size of the queued job.
