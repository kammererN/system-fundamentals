// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-03
/* 
 *   ICSI 333 Lab 05.
 *   Create a program that generates random number between 0 and 50 and puts 
 *   them into a linked list. When it generates 49, instead of adding it to 
 *   the list, it prints the list, freeing each node and then exits. Submit 
 *   your .c file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structs & consts
const int EXIT_VALUE = 49;  // Value at which program exits
const int RAND_CEILING = 51;  // Value+1 of max random number 

typedef struct Node {
	float data;
	struct Node *next;
} Node;

// Prototypes
Node *createNode(float data);
void addNode(Node **head, float data);
void printAndFreeList(Node *head);

// Function to create a new node
Node *createNode(float data)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("Memory allocation failure.\n");
		exit(1); // Exit on malloc failure
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

// Function to add node to the linked list
void addNode(Node **head, float data)
{
	Node *newNode = createNode(data);
	if (*head == NULL) {
		*head = newNode;
	} else {
		Node *temp = *head;
		while (temp ->next != NULL) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}

// Function to print and free the linked list
void printAndFreeList(Node* head)
{
	Node *temp = head;
	while (temp != NULL) {
		printf("%.2f -> ", temp->data);
		Node *prev = temp;
		temp = temp->next;
		free(prev);
	}
	printf("NULL\n");
}

// Program entrypoint
int main()
{
	Node *head = NULL;
	srand(time(0)); // Init rand
	while (1) {
		float randomNumber = rand() % RAND_CEILING;
		if (randomNumber == EXIT_VALUE) {
			printAndFreeList(head);  // If EXIT_VAL, print & exit
			break;
		} else {
			addNode(&head, randomNumber);  // Else add num to list
		}
	}
	return 0;
}
