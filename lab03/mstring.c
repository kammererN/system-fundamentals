// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-10
/* 
 *   ICSI 333 Lab 03. Manual implementation of string function(s).
 */

#include <stdio.h>
#include <string.h>
#define STRING_SIZE 64 // size of memory allocated to string array


/* Manual implementation of strlen() */
// "Calculate the length of the string (without using standard functions)."
size_t mstrlen (char *string) {
	char *pstring = string;
        
        while (*pstring != '\0') {
                pstring++;
        }
        return pstring - string;
}


int main () {
        char string[STRING_SIZE];
        long length, standardLength;

        printf("Enter a string: ");
        // "Using scanf(), read a string from the user."
        scanf("%s", &string);
        length = mstrlen(string);
        standardLength = strlen(string);
        // "Print your calculated length of the string, "
        printf("Calculated length of string \'%s\': %1d\n", string, length); 
        // "followed by the length produced by the standard functions." 
        printf("Standard function returns length: %1d\n", standardLength);
        return 0;
}
