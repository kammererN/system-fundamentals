// SPDX-License-Identifier: GPL-3.0-or-later
// Author: Nicholas Kammerer
// Date: 2024-04-01
/*
 * Write a program that optionally accepts an address and a port from the 
 * command line.
 * 
 * If there is no address and port on the command line, it should create a TCP 
 * socket and print the address (i.e. server mode).
 * 
 * If there is an address and port, it should connect to it (i.e. client mode).
 * Once the connections are set up, each side should enter a loop of receive, 
 * print what it receives, then send a message.
 * 
 * The message should be “ping” from the client and “pong” from the server.
 * In order to test this, you will need to run the same program twice. I had two
 * command line windows open to run mine.
 * 
 * Run first in server mode, then run in client mode using the information 
 * printed from the server as your command line arguments.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5000

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 3) { // Server mode
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        
        memset((char *) &serv_addr, 0, sizeof(serv_addr));
        portno = DEFAULT_PORT;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
        
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
             error("ERROR on accept");
        
        printf("Server Mode: Listening on %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
        
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            n = read(newsockfd, buffer, BUFFER_SIZE-1);
            if (n < 0) error("ERROR reading from socket");
            printf("Here is the message: %s\n", buffer);
            n = write(newsockfd, "pong", 4);
            if (n < 0) error("ERROR writing to socket");
        }
    } else { // Client mode
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        
        memset((char *) &serv_addr, 0, sizeof(serv_addr));
        portno = atoi(argv[2]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
            error("ERROR, no such host");
        }
        
        if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        
        printf("Client Mode: Connected to %s:%d\n", argv[1], portno);
        
        while (1) {
            n = write(sockfd, "ping", 4);
            if (n < 0) error("ERROR writing to socket");
            memset(buffer, 0, BUFFER_SIZE);
            n = read(sockfd, buffer, BUFFER_SIZE-1);
            if (n < 0) error("ERROR reading from socket");
            printf("Received: %s\n", buffer);
        }
    }
    close(sockfd);
    return 0;
}
