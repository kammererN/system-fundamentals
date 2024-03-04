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

// Constants
const int MAX_INPUT = 100;
const int ALPHABET_SIZE = 26;
const char DEFAULT_PUZZLE[] = "This is a test puzzle."
const char STARTUP_PROMPT[] = "Enter decryption pair or 'quit' to exit: "
const char EXIT_PROMPT[] = "Exiting game.\n"

// Global variables
char encryptionKey[ALPHABET_SIZE];

// Prototypes
