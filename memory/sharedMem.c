/*
Shared Memory Assignment - Operating Systems
Use shared memory to create a buffer shared between two processes: a child and parent process.
Code outline by: ChatGPT
Updated by: Mia Weber
10/29/2023
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define BUFFER_SIZE 10 // Buffer that's shared between child and parent process
#define INCREMENT_VALUE 5

// Fill the buffer with the numbers 0-9
void fill_buffer(int *buffer, int size) {
    for (int i = 0; i < size; ++i) {
        buffer[i] = i;
    }
}

// Increment each element in the buffer by 5
void increment_buffer(int *buffer, int size, int increment) {
    for (int i = 0; i < size; ++i) {
        buffer[i] += increment;
    }
}

int main() {
    int shm_fd;
    int *buffer;

    // Create a shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Resize the shared memory object
    if (ftruncate(shm_fd, BUFFER_SIZE * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory object
    buffer = (int *)mmap(0, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        fill_buffer(buffer, BUFFER_SIZE);
        printf("Child filled the buffer with sequential numbers.\n");
    } else { // Parent Process
        // Wait for child process to complete
        wait(NULL);

        printf("Parent is incrementing the buffer's contents by %d...\n", INCREMENT_VALUE);
        increment_buffer(buffer, BUFFER_SIZE, INCREMENT_VALUE); // The child should increment the element

        for (int i = 0; i < BUFFER_SIZE; ++i) {
            printf("buffer[%d] = %d\n", i, buffer[i]); // Print out the updated elements of the buffer
        }
    }

    // Unmap the shared memory object
    if (munmap(buffer, BUFFER_SIZE * sizeof(int)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    // Close the shared memory object
    if (close(shm_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Only the parent process can unlink shared memory -> solves problem of premature unlink
        // Delete the shared memory object
        if (shm_unlink(SHM_NAME) == -1) {
            perror("shm_unlink");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
