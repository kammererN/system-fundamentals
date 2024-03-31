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
 