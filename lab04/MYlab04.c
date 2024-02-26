// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Lab 04. File reader.   
 */
#include <stdio.h>

const size_t FILENAME_SIZE = 64;
const size_t BUFFER_SIZE = 10000000; // accept buffer of up to 10 MB

/*
Write a program that reads a file, line by line, and prints every line on screen word by word
starting every word from new line (any sequence delimited by one or more white symbols is
considered as a word). Submit your .c file
*/

int main(int argc, char* argv[])
{ 
	FILE* file;
	char* filename[FILENAME_SIZE];
	char buffer[BUFFER_SIZE];
	
	// if no args presented, prompt user for filename	 
	if (argc == 1) {
		printf("Enter name of file: ");
		scanf("%63s", filename);
	} else {
	*filename = argv[1];
	}
	// printf("%s\n", filename); // DEBUG

	file = fopen(filename, "r");

	// file error handling if filename is bad
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), file)!= NULL) {
		printf("%s", buffer);
	}

	fclose(file);
	
	return 0;
}