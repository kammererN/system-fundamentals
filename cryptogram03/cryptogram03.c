// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-17
/* 
 *   ICSI 333 Cryptogram game implementation in C, part 03. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_INPUT 100
#define ALPHABET_SIZE 26
#define PUZZLE_FILE_PATH "quotes.txt"

// Constants
const char STARTUP_PROMPT[] = "Enter decryption pair or 'quit' to exit: ";
const char EXIT_PROMPT[] = "Exiting game.\n";
const char PLAY_AGAIN_PROMPT[] = "Would you like to play again? (yes/no): ";

// Structs
typedef struct Quote {
        char *phrase;
        char *author;
        struct Quote *next;
} Quote;

// Global variables
char encryptionKey[ALPHABET_SIZE];
char playerKey[ALPHABET_SIZE];
Quote *quotesHead = NULL;
int quotesCount = 0;
char *encryptedPuzzle;
Quote *currentQuote = NULL;

// Prototypes
void initialization();
void gameLoop();
void tearDown();
void loadPuzzles();
void freeQuotes();
Quote *allocateQuote();
bool displayWorld();
char *acceptInput();
bool updateState(char *input);
void shuffle(char array[], int n);
void encryptPuzzle(const char *input, char **encryptedOutput);
Quote *getPuzzle();

int main()
{
        char response[MAX_INPUT];
        bool playAgain = true;

        while (playAgain) {
                initialization();
                gameLoop();
                tearDown();

                printf("%s", PLAY_AGAIN_PROMPT);
                fgets(response, MAX_INPUT, stdin);
                playAgain = (strncmp(response, "yes", 3) == 0);

                if (playAgain) {
                        freeQuotes();
                }
        }
        freeQuotes(); // Free all quotes at the end
        return 0;
}

void initialization()
{
        srandom(time(NULL)); // Set the seed

        if (quotesCount == 0) {
                loadPuzzles();
        }
        currentQuote = getPuzzle();

        // Generate and shuffle the encryption Key
        for (int i = 0; i < ALPHABET_SIZE; i++) {
                encryptionKey[i] = 'A' + i;
        }
        shuffle(encryptionKey, ALPHABET_SIZE);

        // Initialize player key
        memset(playerKey, '\0', ALPHABET_SIZE);

        // Encrypt the puzzle
        encryptPuzzle(currentQuote->phrase, &encryptedPuzzle);
}

void gameLoop()
{
        char *userInput;
        bool quit = false;
        bool complete = false;

        while (!quit && !complete) {
                complete = displayWorld();
                if (!complete) {
                        userInput = acceptInput();
                        quit = updateState(userInput);
                }
        }

        if (complete) {
                printf("Congratulations, you've solved the puzzle!\n");
                printf("%s\n", currentQuote->author); // Display the author when game is complete
        }
}

void tearDown()
{
        printf("%s\n", EXIT_PROMPT); // Move "Exiting game" message to tearDown
        free(encryptedPuzzle);
        encryptedPuzzle = NULL;
}

void loadPuzzles()
{
        FILE *file = fopen(PUZZLE_FILE_PATH, "r");
        if (file == NULL) {
                perror("Failed to open puzzle file");
                exit(1);
        }
        char line[MAX_INPUT];
        Quote *last = NULL; // Add a pointer to keep track of the last quote added

        while (fgets(line, sizeof(line), file) != NULL) {
                if (strlen(line) < 3) { // New quote entry
                        quotesCount++;
                } else if (line[0] == '-') { // Author line
                        line[strcspn(line, "\n")] = '\0'; // Remove newline from author
                        if (last) {
                                last->author = strdup(line + 2); // Skip "-- " before author's name
                        }
                } else { // Part of the phrase
                        Quote *current = allocateQuote();
                        current->phrase = strdup(line);
                        if (last == NULL) { // First quote
                                quotesHead = current; 
                        }
                        last = current; // Update last to current quote
                }
        }
        fclose(file);
}

Quote *allocateQuote()
{
        Quote *newQuote = (Quote *)malloc(sizeof(Quote));
        newQuote->phrase = NULL;
        newQuote->author = NULL;
        newQuote->next = quotesHead;
        quotesHead = newQuote;
        return newQuote;
}

bool displayWorld()
{
        bool complete = true;
        printf("Encrypted: %s\n", encryptedPuzzle);
        printf("Decrypted: ");
        for (size_t i = 0; encryptedPuzzle[i] != '\0'; i++) {
                if (isalpha(encryptedPuzzle[i])) {
                        int index = encryptedPuzzle[i] - 'A';
                        char decryptedChar = playerKey[index];
                        printf("%c", decryptedChar ? decryptedChar : '_');
                        if (decryptedChar == '\0' || decryptedChar != currentQuote->phrase[i]) {
                                complete = false; // Mark incomplete if any character doesn't match or isn't decoded
                        }
                } else {
                        printf("%c", encryptedPuzzle[i]);
                }
        }
        printf("\n");
        return complete;
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

void shuffle(char array[], int n)
{
        for (int i = n - 1; i > 0; i--) {
                int j = random() % (i + 1);
                char temp = array[i];
                array[i] = array[j];
                array[j] = temp;
        }
}

void encryptPuzzle(const char *input, char **encryptedOutput)
{
        size_t length = strlen(input);
        *encryptedOutput = (char *)malloc(length + 1); // Malloc for puzzle

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

void freeQuotes()
{
        Quote *current = quotesHead;
        while (current != NULL) {
                Quote *next = current->next;
                free(current->phrase);
                free(current->author);
                free(current);
                current = next;
        }
        quotesHead = NULL;
        quotesCount = 0;
}

Quote *getPuzzle()
{
        if (quotesCount == 0) {
                loadPuzzles(); // Ensure puzzles are loaded if not already done
        }
        int randomIndex = random() % quotesCount;
        Quote *current = quotesHead;
        for (int i = 0; i < randomIndex && current != NULL; i++) {
                current = current->next;
        }
        return current;
}