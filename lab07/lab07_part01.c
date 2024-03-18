// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-17
/* 
 * ICSI 333 Lab 07, part 01
 *     Write a program that creates an array of 100 random numbers from 0-99.
 *     The program sums the random numbers and prints the sum.
 *     It then writes the numbers to a new file using open, close and write
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
    srand(time(0)); // Initialize the random number generator
    
    int numbers[100];
    int sum = 0;
    
    for (int i = 0; i < 100; i++) {
        numbers[i] = rand() % 100; // Generate a random number between 0-99
        sum += numbers[i];
    }
    
    printf("Sum = %d\n", sum); // Print the sum
    
    // Create the filename
    char filename[20];
    sprintf(filename, "numbers.%d", sum);
    
    // Open the file for writing
    int fd = open(filename, O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }
    
    // Write the numbers to the file
    for (int i = 0; i < 100; i++) {
        char buffer[10];
        int len = sprintf(buffer, "%d\n", numbers[i]);
        write(fd, buffer, len);
    }
    
    // Close the file
    close(fd);
    
    return 0;
}
