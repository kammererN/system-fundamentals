// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Lab 04. File reader.   
 */
#include <stdio.h>
~e#include <string.h>

const size_t LINE_SIZE = 1024; 
const size_t FILENAME_BUFFER_SIZE = 64;

/*
Write a program that reads a file, line by line, and prints every line on screen word by word
starting every word from new line (any sequence delimited by one or more white symbols is
considered as a word). Submit your .c file
*/

int main(int argc, char* argv[])
{ 
	FILE* file;
	char* filename;
	char line[LINE_SIZE];

	filename = argv[1];

	file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error opening file");
		return -1;
	}

    	// read the file line by line
	while (fgets(line, sizeof(line), file)) {
		char *word = strtok(line, " \t\n\r");
		while (word != NULL) {
			printf("%s\n", word);
			word = strtok(NULL, " \t\n\r");
		}
	}
	fclose(file);
	return 0;
}