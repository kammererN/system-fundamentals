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
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	int numbers[100]; // Array to hold 100 random numbers
	int sum = 0; // Variable to hold the sum of the numbers
	char filename[20]; // Array to hold the filename

	srand(time(0)); // Init random number generator

	// Generate 100 random ints from 0-99 and sum them
	for (int i = 0; i < 100; i++) {
		numbers[i] = rand() % 100; // Generate rand int 0-99
		sum += numbers[i]; // Add number to sum
	}

	printf("Sum of numbers: %d\n", sum); // Print the sum

	// Construct filename using the sum
	sprintf(filename, "numbers.%d", sum);

	// Open file for writing
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		perror("Error opening file");
		return 1;
	}

	// Convert each number to a string and write to the file
	for (int i = 0; i < 100; i++) {
		char numStr[5]; // Buffer for number string
		sprintf(numStr, "%d\n", numbers[i]); // Convert number to string
		write(fd, numStr, strlen(numStr)); // Write string to file
	}

	close(fd); // Close the file

	return 0; // End of program
}