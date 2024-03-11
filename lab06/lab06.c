// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-10
/* 
 *   ICSI 333 Lab 06
 *   Write a program that takes a number from the command line.
 *   It should print:
 *	The number.
 * 	The number of bits in the number that are set to 0 and 1.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    unsigned long number = strtoul(argv[1], NULL, 10); // Convert the first argument to an unsigned long
    unsigned int count_0 = 0, count_1 = 0;
    unsigned long temp = number;

    // Count bits set to 0 and 1
    while (temp > 0) {
        if (temp & 1) // If the least significant bit is 1
            count_1++;
        else
            count_0++;
        temp >>= 1; // Right shift the bits of temp by 1
    }

    unsigned int total_bits = sizeof(number) * 8; // Total bits in an unsigned long
    count_0 += total_bits - (count_0 + count_1); // Adjust count_0 for leading zeros

    printf("Your number was %lu\n", number);
    printf("In %lu, there are %u bits set to 0.\n", number, count_0);
    printf("In %lu, there are %u bits set to 1.\n", number, count_1);

    return 0;
}
