// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-02-11
/* 
 *   ICSI 333 Cryptogram 01 game implementation in C. 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_INPUT 128 
#define DEFAULT_PUZZLE "Hello! this is the puzzle :)"
#define QUIT "quit"
#define STARTUP_PROMPT "Enter a letter and then the letter to replace it with or 'quit' to quit: "
#define EXIT_PROMPT "All Done\n"

// Global var to hold puzzle
char *puzzle;

// Function prototypes
void initialization();
void gameLoop();
void tearDown();
char *getPuzzle(); 
char *acceptInput();
bool updateState(char *input);
void displayWorld();

int main () 
{
	initialization();
	gameLoop();
	tearDown();
	return 0;
}


void initialization()
{
	puzzle = getPuzzle();
}


void gameLoop()
{
	char *userInput;
	bool quit = false;

	while (!quit) {
		displayWorld(); 
		userInput = acceptInput();
		quit = updateState(userInput);
	}
}


void tearDown()
{
	printf(EXIT_PROMPT);
}


char *getPuzzle()
{
	return DEFAULT_PUZZLE;
}


char *acceptInput()
{
	static char input[MAX_INPUT];
	printf(STARTUP_PROMPT);
	fgets(input, MAX_INPUT, stdin);
	input[strcspn(input, "\n")] = 0;  // Remove "/n" if present
	return input;
}


bool updateState(char *input)
{
	if (strcmp(input, QUIT) == 0 || input == NULL) {
		return true; // quits game
	}

	return false;
}

void displayWorld()
{
	printf("Puzzle: %s\n", puzzle);
}
