// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Lab 04. File reader.   
 */
#include <stdio.h>

const size_t FILENAME_SIZE = 64; 
/*
Write a program that reads a file, line by line, and prints every line on screen word by word
starting every word from new line (any sequence delimited by one or more white symbols is
considered as a word). Submit your .c file
*/

int main(int argc, char* argv[])
{ 
	FILE* fp;
	char* filename[FILENAME_SIZE];

	// if no args presented, prompt user for filename	 
	if (argc == 1) {
		printf("Enter name of file: ");
		scanf("%63s", filename);
	} else {
		*filename = argv[1];
	}
	printf("%s\n", filename); // DEBUG

	//fp = fopen(*filename, "r"); 
	// fclose(fp);
	
	return 0;
}