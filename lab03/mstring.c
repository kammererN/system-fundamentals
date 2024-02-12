// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-10
/* 
 *   ICSI 333 Lab 03. Manual implementation of string function(s).
 */

#include <stdio.h>
#define STRING_SIZE 64 // size of memory allocated to string array


/* Manual implementation of strlen() */
size_t mstrlen (char *string) {
	char *pstring = string;
        
        while (*pstring != '\0') {
                pstring++;
        }
        return pstring - string;
}


int main () {
        char string[STRING_SIZE];
        long length;

        printf("Enter a string: ");
        scanf("%s", &string);
        length = mstrlen(string);
        printf("String \"%s\" has length of %1d.\n", string, length); 
        return 0;
}
