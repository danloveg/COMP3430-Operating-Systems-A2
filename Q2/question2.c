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

    // Demonstrate that there is only one kernel level thread for the process.
    // We do this by effectively running:
    //         ps axo pid,nlwp | grep -E 'PID NLWP|^ rootPID' > q2output.txt
    rootPID = getpid();
    pipe(fd);
    pid = fork();
    assert(pid != -1);

    if (pid != 0) {
        // Fork again
        pid = fork();
        assert(pid != -1);

        if (pid == 0) {
            // Send stdout to fd[WRITE]
            close(fd[READ]);
            dup2(fd[WRITE], STDOUT_FILENO);
            close(fd[WRITE]);

            // Execute ps program and display the PID & NLWP
            returnStatus = execv("/bin/ps", (char *[]){"/bin/ps", "axo", "pid,nlwp", 0});
            exit(returnStatus);
        } else {
            // Open a file
            int fileDescriptor = open(FILENAME, O_WRONLY|O_CREAT, 0666);

            if (fileDescriptor != -1) {
                // Get stdin from fd[READ], send stdout to a file
                close(fd[WRITE]);
                dup2(fd[READ], STDIN_FILENO);
                close(fd[READ]);
                dup2(fileDescriptor, STDOUT_FILENO);

                // Print a title for the file (stdout is being piped to file)
                printf("Process Stats\n");

                // Create argument array for grep
                char buf[20]; char *pattern = &buf[0];
                sprintf(pattern, "PID NLWP|^ *%d", rootPID);
                char *args[] = {"/bin/grep", "-E", pattern, 0};

                // Execute grep to find our process
                returnStatus = execv("/bin/grep", args);
                exit(returnStatus);
            } else {
                // Error opening file, just exit
                exit(1);
            }
        }
    } else {
        // Wait on processes to complete
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
