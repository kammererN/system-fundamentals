// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Cryptogram 01. Game implementation in C. 
 */

#include <stdio.h>
#define STRING_LENGTH 128 

char puzzle[STRING_LENGTH];

const char getPuzzle() 
{
	const char string[STRING_LENGTH] = "Welcome to Cryptogram 01!";

	return *string;
}

void initialization()
{
	puzzle = *getPuzzle();
}


int main () 
{
	//initialization();
	//gameloop();
	//teardown();
	return 0;
}