#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    pid_t unknown_pid;  // Process ID
    pid_t parent_pid;
    pid_t child_pid[n];

    printf("n=%i\n", n);

    for (int i = 0; i < n; i++) {  // 3 times to create 3 child processes (total of 4 processes including the initial one)
        //parent_pid=getpid();
        //unknown_pid = fork();
        /////printf("i=%i\n", i);
        printf("Process #%i\n", i+1);

        //////printf("Original process id = %d\n", getpid());
        unknown_pid = fork();
        //printf("Process #%i: PID=%d\n", i, getpid());

        if (unknown_pid > 0) {
            child_pid[0] = unknown_pid;
            printf("I am the parent process.\n");
            printf("My process ID is: %i\n", unknown_pid);
            //printf("Parent Process ID: %d, Child Process ID: %d\n",parent_pid, child_pid[0]);
        } else if (unknown_pid == 0) {
            child_pid[0] = getpid();
            printf("I am the child process.\n");
            printf("My process ID is: %i\n", unknown_pid);
            //printf("Parent Process ID: %d, Child Process ID: %d\n", parent_pid, child_pid[0]);
            
            if (execl("./hello", "hello", NULL) == -1) {
                perror("exec");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Fork Failed!\n");
        }
        //printf("Process#%i ID:%d\n", i, getpid());

        //return 0;

    }

    return 0;
}
