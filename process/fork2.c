#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); // Number of children processes
    pid_t unknown_pid;  // Process ID

    printf("I am the original parent process. My process ID is: %d\n", getpid());

    for (int i=0; i<n; i++) {
        unknown_pid = fork();

        if (unknown_pid == 0) {
            printf("I am the child process. My process ID is: %d.", getpid());
            printf(" Parent process ID is: %d.\n", getppid());
            exit(0);
        } else if (unknown_pid > 0) {
           // The parent process. Nothing to do.
        } else {
            printf("Fork Failed!\n");

        }
    }

    for (int i=0; i<n; i++) { // Wait for the children process to execute
        wait(NULL);
    }

    return 0;
}