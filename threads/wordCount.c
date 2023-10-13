/*
    Word Count program as suggested by ChatGPT conversation
    Counts the occurences of each word in an arrary using threads and mutex
    Updated and modified by: Mia Weber
    10/13/2023
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // Use the pthread library

#define NUM_THREADS 4 // Determine the number of threads to create

char *words[] = { // Array of words to parse 
    "apple", "bannana", "cherry", "date",
    "bannana", "apple", "date", "cherry",
    "date", "date", "apple", "cherry",
    "date", "apple", "bannana", "date"
};

struct ThreadInfo { // This info is passed to each thread
    int start; 
    int end; 
    int *counters; // Keep track of occurances 
    pthread_mutex_t *mutex; // Mutex to avoid destroying shared resources 
};

void *countWords(void *threadArg) { // Function used to count occurences of word
    struct ThreadInfo *data;
    data = (struct ThreadInfo *) threadArg;

    for (int i = data->start; i < data->end;i++) {
        pthread_mutex_lock(data->mutex); // Counters is a shared resource. Lock the mutex when updating it

        if (strcmp(words[i], "apple") == 0) {
            data->counters[0]++;
        } else if (strcmp(words[i], "bannana") == 0) {
            data->counters[1]++;
        } else if (strcmp(words[i], "cherry") == 0) {
            data->counters[2]++;
        } else if (strcmp(words[i], "date") == 0) {
            data->counters[3]++;
        }
        pthread_mutex_unlock(data->mutex); // Done modifying shared resource, unlock the mutex
    }
    pthread_exit(NULL); // Thread has finished, exit
} 


int main() {
    pthread_t threads[NUM_THREADS]; // Holds thread identifiers 
    struct ThreadInfo threadInfo[NUM_THREADS]; // Holds thread data (ThreadInfo struct above)
    int counters[4] = {0,0,0,0}; // For "apple", "bannana", "cherry", and "date"

    pthread_mutex_t mutex; // Use a mutex to protect shared resources (counters array)
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex

    // Each thread will work with a section of the words array
    // Calculate how big of a section each thread will work with
    int wordsLength = sizeof(words) / sizeof(words[0]);
    int chunkSize = wordsLength / NUM_THREADS; // Divide the words array into chunks for each thread

    // Create the threads and assign their info (start, end, counters, mutex)
    for (int i=0; i< NUM_THREADS; ++i) {
        threadInfo[i].start = i * chunkSize;
        threadInfo[i].end = (i+1) * chunkSize;
        threadInfo[i].counters = counters;
        threadInfo[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, countWords, (void *)&threadInfo[i]);
    }

    for (int i=0; i< NUM_THREADS; ++i) { // Wait for the threads to complete
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // Destroy the mutex

    // Print the final word counts to the terminal
    printf("apple: %d\n", counters[0]);
    printf("bannana: %d\n", counters[1]);
    printf("cherry: %d\n", counters[2]);
    printf("date: %d\n", counters[3]);

    return 0;
}