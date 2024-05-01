// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerers
// Date: 2024-04-08
/* 
 * Write a program that creates an array of 1,000,000 random integers 0-9.
 * Sum those integers and print the sum.
 * It should then create two threads, which iterate over ½ of the integers, 
 * summing them using a single global variable.
 * When both threads complete, the main thread should print the global variable
 * Hint – the threaded sums should be less than the “real” answer.
 * In the threads, wrap the access to the single global variable with a 
 * semaphore. Rerun. Change the threads to use a local variable for summing, 
 * and then update the global with the semaphore lock. Rerun.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_INTS 1000000
int randomInts[NUM_INTS];
long long sum = 0; // To hold the final sum
sem_t mutex;

void* sumArrayImproved(void* arg) {
    long start = (long)arg;
    long end = start + (NUM_INTS / 2);
    long long localSum = 0;
    for (long i = start; i < end; i++) {
        localSum += randomInts[i];
    }
    sem_wait(&mutex);
    sum += localSum;
    sem_post(&mutex);

    return NULL;
}

int main() 
{
	// Generate random integers between 0-9
    	for (int i = 0; i < NUM_INTS; i++) {
        	randomInts[i] = rand() % 10;
    	}

    	// Sum the integers without threads
    	long long singleThreadSum = 0;
    	for (int i = 0; i < NUM_INTS; i++) {
        	singleThreadSum += randomInts[i];
    	}
    	printf("Single-threaded sum: %lld\n", singleThreadSum);

    	sem_init(&mutex, 0, 1); // Initialize semaphore

    	pthread_t threads[2];
    
    	// Creating two threads to sum the array
    	for (long i = 0; i < 2; i++) {
        	pthread_create(&threads[i], NULL, sumArrayImproved, (void*)(i * (NUM_INTS / 2)));
    	}

    	// Wait for threads to finish
    	for (int i = 0; i < 2; i++) {
        	pthread_join(threads[i], NULL);
    	}

    	printf("Multi-threaded sum with semaphore: %lld\n", sum);

    	return 0;
}