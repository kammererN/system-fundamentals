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
#include <dirent.h>
#include <string.h>

void sum_and_print_file_contents(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    
    int sum = 0, num;
    while (fscanf(fp, "%d", &num) > 0) { // Read numbers as integers
        sum += num;
    }
    
    printf("%s: %d\n", filename, sum);
    
    fclose(fp);
}

int main(void) {
    DIR* dir;
    struct dirent* ent;
    
    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strncmp(ent->d_name, "numbers.", 8) == 0) {
                sum_and_print_file_contents(ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
