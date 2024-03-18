// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-17
/* 
 * ICSI 333 Lab 07, part 02
 * Write a program that looks in the current directory for files that match 
 * the pattern “numbers.XXXX”. Use opendir(), readdir(), closedir() for this.
 * For each file, open the file and read the file.
 * You can assume that the file will contain 100 integers. Sum the integers.
 * Print the filename and the sum of the integers.  
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // For directory operations
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// Prototypes
void processFile(const char *filename);

// Function to open, read, sum, print & close the file
void processFile(const char *filename)
{
        int fd = open(filename, O_RDONLY);
	if (fd == -1) {
		perror("Error opening file");
		return;
	}

	int sum = 0, number;
	for (int i = 0; i < 100; i++) {
		// Assuming each integer is stored in a newline-seperated format
		if (read(fd, &number, sizeof(int)) > 0) {
			sum += number;
			printf("%s", sum);
		}
	}

	printf("%s: Sum = %d\n", filename, sum); 

	close(fd);
}

int main(void)
{
	DIR *dir;
	struct dirent *entry;

	dir = opendir("."); // Open the current directory
	if (dir == NULL) {
		perror("opendir() error");
		return 1;
	}

	while ((entry = readdir(dir)) != NULL) {
		// Check if the filename matches the pattern "numbers."
		if (strncmp(entry->d_name, "numbers.", 8) == 0) {
			processFile(entry->d_name);
		}
	}
	closedir(dir);
	
	return 0;
}