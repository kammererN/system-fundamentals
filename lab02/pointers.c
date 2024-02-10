// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-10
/* 
 *   ICSI 333 Lab 02. Exercise in pointers. 
 */
#include <stdio.h>

int main ()
{
    // Allocate two variables
    int a;
    int b;

    // Create two pointers that point to those variables.
    int *x = &a;
    int *y = &b;

    // Set the first variable to a value
    a = 5;

    // Dereference the second pointer to change its value.
    // Print all values
    printf("a = %i\tb = %i\tx = %i\ty = %i\n", a, b, x, *y);

    return 0;
}
