// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-03-29
/* 
 * ICSI 333 Lab 08
 * Write a program where:
 *    a. Main:
 *       i. Creates two pipes and makes them global.
 *       ii. Creates (fork) two processes. Each process should call a function defined
 *           in (b) and (c) below.
 *       iii. Wait() for a process to end, then send the SIGUSR1 signal to the (c)
 *       process before ending.
 *
 *    b. One function should have an integer variable that starts at 0. It should print
 *       “ping – {value}” then increment the value. It should write that value to a pipe
 *       and read the value back from the other pipe until the value is greater than or
 *       equal to 100. It should call exit() when complete.
 *
 *    c. The other function should set up a signal handler for SIGUSR1 to call a function
 *       (defined in (d) below) when it receives that signal. It should then loop forever:
 *       read from a pipe, print “pong-{value}”, increment the value and write the value
 *       to the other pipe. These pipes must be opposite from the function in (b) – the
 *       pipe you write to in (b) must be the pipe that you read from in (c) and vice versa.
 *    
 *    d. Create a function for the signal handler that should print “pong quitting” and
 *       exit().
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

// Global pipe file descriptors
int filedescriptor1[2]; // Used for ping to write & pong to read
int filedescriptor2[2]; // Used for pong to write & ping to read

void pingProcess();
void pongProcess();
void signalHandler(int signal);

int main()
{
	pid_t pid1, pid2;

	// Create the pipes
	if (pipe(filedescriptor1) == -1 || pipe(filedescriptor2) == -1) {
		perror("pipe");
		exit(1);
	}

	// Fork the first child
	pid1 = fork();
	if (pid1 == 0) {
		// Child
		pingProcess();
	} else {
		pid2 = fork();
		if (pid2 == 0) {
			// Child
			pongProcess();
		}
	}

	// Parent waits for child to complete
	wait(NULL);

	// Send SIGUSR1 signal to pong
	kill(pid2, SIGUSR1);

	// Wait for second child to complete
	wait(NULL);

	return 0;
}
 
void pingProcess()
{
        int value = 0;
	while (value <= 100) {
		printf("ping - %d\n", value);
		value++;
		write(filedescriptor1[1], &value, sizeof(value)); // write pong
		read(filedescriptor2[0], &value, sizeof(value)); // read pong
	}
	exit(0);
 }

void pongProcess()
{
        signal(SIGUSR1, signalHandler); // Setup signal handler
	int value;
	while (1) {
		read(filedescriptor1[0], &value, sizeof(value)); // read ping
		printf("pong-%d\n", value);
		value++;
		write(filedescriptor2[1], &value, sizeof(value)); // write ping
	}
}

void signalHandler(int signal)
{
	printf("pong quitting\n");
	exit(0);
}