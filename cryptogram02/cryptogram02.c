// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-01
/* 
 *   ICSI 333 Cryptogram game implementation in C, part 02. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

// Compile-time variables)
#define MAX_INPUT 100
#define ALPHABET_SIZE 26

// Constants
const char DEFAULT_PUZZLE[] = "This is a test puzzle.";
const char STARTUP_PROMPT[] = "Enter decryption pair or 'quit' to exit: ";
const char EXIT_PROMPT[] = "Exiting game.\n";

// Global variables
char encryptionKey[ALPHABET_SIZE];
char playerKey[ALPHABET_SIZE];
char *puzzle;
char *encryptedPuzzle;

// Prototypes
void initialization();
void gameLoop();
void tearDown();
const char *getPuzzle();
char *acceptInput();
bool updateState(char *input);
void displayWorld();
void shuffle(char array[], int n);
void encryptPuzzle(const char *input, char **encryptedOutput);

// Entrance to program
int main() 
{
	initialization();
	gameLoop();
	tearDown();
	return 0;
}

void initialization()
{
	srandom(time(NULL)); // set the seed
	puzzle = strdup(getPuzzle()); // Duplicate the puzzle string

	// Generate and shuffle the encryption key
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		encryptionKey[i] = 'A' + i;
	}
	shuffle(encryptionKey, ALPHABET_SIZE);

	//Initialize player key
	memset(playerKey, '\0', ALPHABET_SIZE);

	// Encrypt the puzzle
	encryptPuzzle(puzzle, &encryptedPuzzle);
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
	displayWorld(); // Display puzzle one last time
	printf("%s", EXIT_PROMPT);
	free(puzzle);
	free(encryptedPuzzle);
}

const char *getPuzzle()
{
	return DEFAULT_PUZZLE;
}

char *acceptInput()
{
	static char input[MAX_INPUT];
	printf("%s", STARTUP_PROMPT);
	fgets(input, MAX_INPUT, stdin);
	input[strcspn(input, "\n")] = '\0'; // Remove "\n" if present
	return input;
}

bool updateState(char *input)
{
	if (strcmp(input, "quit") == 0 || input == NULL) {
		return true; // quits game
	}

	if (strlen(input) == 2) {
		char original = toupper(input[0]);
		char replacement = toupper(input[1]);
		if (isalpha(original) && isalpha(replacement)) {
			int index = original - 'A';
			playerKey[index] = replacement;
		} else {
			printf("Warning: Please enter a valid letter pair.\n");
		}
	} else {
		printf("Error: Input must be exactly 2 characters.\n");
	}
	return false;
}

void displayWorld()
{
	printf("Encrypted: %s\n", encryptedPuzzle);
	printf("Decrypted: ");
	for (size_t i = 0; encryptedPuzzle[i] != '\0'; i++) {
		if (isalpha(encryptedPuzzle[i])) {
			int index = encryptedPuzzle[i] - 'A';
			char decryptedChar = playerKey[index];
			// print decrypted chars using ternary operator
			printf("%c", decryptedChar ? decryptedChar:'_');
		} else {
			printf("%c", encryptedPuzzle[i]);
		}
	}
	printf("\n");
}

// Implementation of Fisher-Yates shuffle algorithm
void shuffle(char array[], int n)
{
	for (int i = n-1; i > 0; i--){
		int j = random() % (i + 1);
		char temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

void encryptPuzzle(const char *input, char **encryptedOutput)
{
	size_t length = strlen(input);
	*encryptedOutput = (char *)malloc(length + 1); // malloc for puzzle
	
	// Uppercase entire string
	for (size_t i = 0; i < length; i++) {
		char c = toupper(input[i]);
		if (isalpha(c)) {
			int index = c - 'A';
			(*encryptedOutput)[i] = encryptionKey[index];
		} else {
			(*encryptedOutput)[i] = input[i];
		}
	}
	(*encryptedOutput)[length] = '\0'; // NUL terminate encrypted string
}