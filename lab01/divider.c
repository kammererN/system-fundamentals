// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-10
/* 
 * ICSI 333 Lab 01. Divide and modulo two integers; return results. 
 *  
 */
#include  <stdio.h>
int main() 
{
  
  int number01;
  int number02;
  int quotient;
  int remainder;

  printf("This program finds the quotient and remainder of two numbers. \n");

  printf("Enter number 01: ");
  scanf("%i",&number01);

  printf("Enter number 02: ");
  scanf("%i",&number02);  

  quotient = number01 / number02;
  remainder = number01 % number02;

  printf("===================================\n"); 
  printf("The quotient of %i and %i is %i, with a remainder of %i.\n", number01, number02, quotient, remainder);

  // Return success
  return(0);
}
