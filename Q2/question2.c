#include <stdio.h>
#include <assert.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define FILENAME "./q2output.txt"
#define READ 0
#define WRITE 1

void *dummyThread(void *arg);

int main(int argc, char *argv[]) {
    int fd[2];
    int pid, returnStatus;
    pid_t rootPID;
    pthread_t threads[2];

    // Create two threads
    printf("Creating two threads...\n");
    pthread_create(&threads[0], NULL, dummyThread, NULL);
    pthread_create(&threads[1], NULL, dummyThread, NULL);

    // Demonstrate that there is only one LWP for the process
    rootPID = getpid();
    pipe(fd);
    pid = fork();
    assert(pid != -1);

    if (pid != 0) {
        // Fork again
        pid = fork();
        assert(pid != -1);

        if (pid == 0) {
            // Close read part of pipe
            close(fd[READ]);
            // Close stdout, duplicate with write part of pipe
            dup2(fd[WRITE], STDOUT_FILENO);
            close(fd[WRITE]);

            // Execute ps program and display the PID & NLWP
            returnStatus = execv("/bin/ps", (char *[]){"/bin/ps", "axo", "pid,nlwp", 0});
            exit(returnStatus);
        } else {
            // Open a file
            int fileDescriptor = open(FILENAME, O_WRONLY|O_CREAT, 0666);

            if (fileDescriptor != -1) {
                // Close write part of pipe
                close(fd[WRITE]);
                // Close stdin, replace with read part of pipe
                dup2(fd[READ], STDIN_FILENO);
                close(fd[READ]);
                // Set stdout to the file we opened
                dup2(fileDescriptor, STDOUT_FILENO);
                // Create argument array for grep
                char buf[20]; char *pattern = &buf[0];
                sprintf(pattern, "PID NLWP|^ %d", rootPID);
                char *args[] = {"/bin/grep", "-E", &pattern[0], 0};
                // Print a title for the file
                printf("Process Stats\n");

                // Execute grep to find our process
                returnStatus = execv("/bin/grep", args);
                exit(returnStatus);
            } else {
                // Error opening file, just exit
                exit(1);
            }
        }
    } else {
        waitpid(pid, &returnStatus, 0);
    }

    // Wait for threads to exit
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("Exiting.\n");

    pthread_exit(0);
}

void *dummyThread(void *arg) {
    int i;
    // Spin
    for (i = 0; i < 1000000; i++);
    
    return NULL;
}
