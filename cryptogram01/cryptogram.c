// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Cryptogram 01. Game implementation in C. 
 */

#include <stdio.h>

#define STRING_LENGTH 128 
#define DEFAULT_STRING "Hey there ;)"
#define STARTUP_PROMPT "Enter a letter and then the letter to replace it with or ':q' to quit."
#define EXIT_PROMPT "All Done"

// Function declarations
void initialization();
void gameLoop();
void teardown();
char getPuzzle(); 
char acceptInput();
void updateState(char *userInputq);
void displayWorld();

// Universal vars
char puzzle[STRING_LENGTH];


/* Main function */
int main () 
{
	initialization();
	gameLoop();
	teardown();
	return 0;
}


void initialization()
{
	puzzle = *getPuzzle();
}


void gameLoop()
{

}


void teardown()
{
	printf(EXIT_PROMPT);
}


char getPuzzle()
{
	const char string[STRING_LENGTH] = DEFAULT_STRING;
	return *string;
}


char acceptInput()
{
	char userInput[STRING_LENGTH];

	printf(STARTUP_PROMPT);
	fgets(&userInput, STRING_LENGTH, stdin);
	return *userInput;
}


bool updateState(char *userInput)
{
	char line[STRING_LENGTH];

	if (*userInput != NULL || ':q') {
		
		return true;
	} else {
		displayWorld();
		return false;
	}
}

void displayWorld()
{
	printf("");
}
