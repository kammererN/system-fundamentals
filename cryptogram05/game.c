#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define PORT "8000"
#define BUFFER_SIZE 1024
#define RESPONSE_HEADER_TEMPLATE "HTTP/1.0 %s\r\nContent-Length: %ld\r\n\r\n"
#define NOT_FOUND_RESPONSE "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n"
#define OK_STATUS "200 OK"
#define NOT_FOUND_STATUS "404 Not Found"
#define HTML_CONTENT_TYPE_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"

#define MAX_INPUT 100
#define ALPHABET_SIZE 26
#define PUZZLE_FILE_PATH "quotes.txt"

typedef struct Quote {
    char *phrase;
    char *author;
    struct Quote *next;
} Quote;

char encryptionKey[ALPHABET_SIZE];
char playerKey[ALPHABET_SIZE];
Quote *quotesHead = NULL;
int quotesCount = 0;
char *encryptedPuzzle;
Quote *currentQuote = NULL;

void loadPuzzles();
void shuffle(char array[], int n);
void encryptPuzzle(const char *input, char **encryptedOutput);
Quote *getPuzzle();
void freeQuotes();
bool updateState(char *input);
bool isGameOver();

void *handle_request(void *arg);
int setup_server_socket(void);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path to serve>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *base_path = argv[1];

    loadPuzzles(); // Pre-load puzzles

    int server_fd = setup_server_socket();
    while (1) {
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        int new_fd = accept(server_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        char *client_data = malloc(strlen(base_path) + BUFFER_SIZE);
        strcpy(client_data, base_path);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_request, client_data)) {
            perror("pthread_create");
            close(new_fd);
        }
        pthread_detach(thread_id);
    }

    return 0;
}

void *handle_request(void *arg)
{
    char *base_path = (char *)arg;
    char buffer[BUFFER_SIZE] = {0};
    int client_fd = *((int *)arg);
    free(arg);

    read(client_fd, buffer, sizeof(buffer));

    char *method, *path, *version, *saveptr;
    method = strtok_r(buffer, " ", &saveptr);
    path = strtok_r(NULL, " ", &saveptr);
    version = strtok_r(NULL, " ", &saveptr);

    if (method && strcmp(method, "GET") == 0 && path) {
        if (strncmp(path, "/crypt", 6) == 0) {
            handleGame(client_fd, path);
        } else {
            char *full_path = malloc(strlen(base_path) + strlen(path) + 1);
            sprintf(full_path, "%s%s", base_path, path + 1); 
            respond_with_file(client_fd, full_path);
            free(full_path);
        }
    } else {
        respond_not_found(client_fd);
    }
    close(client_fd);
    return NULL;
}

int setup_server_socket(void)
{
    struct addrinfo hints, *res;
    int server_fd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        close(server_fd);
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1) {
        close(server_fd);
        perror("listen");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
    return server_fd;
}

void handleGame(int client_fd, char *path) {
    if (strstr(path, "?move=")) {
        char *move = strpbrk(path, "=") + 1;
        if (strlen(move) == 2 && updateState(move)) {
            if (isGameOver()) {
                char *html = "<html><body>Congratulations! You solved it! <a href=\"crypt\">Another?</a></body></html>";
                char header[BUFFER_SIZE];
                sprintf(header, HTML_CONTENT_TYPE_HEADER);
                write(client_fd, header, strlen(header));
                write(client_fd, html, strlen(html));
            } else {
                displayWorld(client_fd);
            }
        } else {
            displayWorld(client_fd);
        }
    } else {
        currentQuote = getPuzzle();
        encryptPuzzle(currentQuote->phrase, &encryptedPuzzle);
        displayWorld(client_fd);
    }
}

void displayWorld(int client_fd) {
    char html[2048];
    sprintf(html, "<html><body>Encrypted: %s\n<P> Decrypted: ", encryptedPuzzle);
    for (int i = 0; encryptedPuzzle[i] != '\0'; i++) {
        if (isalpha(encryptedPuzzle[i])) {
            int index = encryptedPuzzle[i] - 'A';
            char decryptedChar = playerKey[index];
            char append[2] = {decryptedChar ? decryptedChar : '_', '\0'};
            strcat(html, append);
        } else {
            char append[2] = {encryptedPuzzle[i], '\0'};
            strcat(html, append);
        }
    }
    strcat(html, "<form submit=\"crypt\"><input type=\"text\" name=move autofocus maxlength=2></input></form></body></html>");
    char header[BUFFER_SIZE];
    sprintf(header, HTML_CONTENT_TYPE_HEADER);
    write(client_fd, header, strlen(header));
    write(client_fd, html, strlen(html));
}

bool updateState(char *input) {
    if (input == NULL) {
        return false;
    }
    if (strlen(input) == 2) {
        char original = toupper(input[0]);
        char replacement = toupper(input[1]);
        if (isalpha(original) && isalpha(replacement)) {
            int index = original - 'A';
            playerKey[index] = replacement;
            return true;
        }
    }
    return false;
}

bool isGameOver() {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (playerKey[i] == '\0') {
            return false;
        }
    }
    return true;
}

void loadPuzzles() {
    /* Dummy function to simulate loading puzzles */
    quotesHead = malloc(sizeof(Quote));
    quotesHead->phrase = "HELLO WORLD";
    quotesHead->author = "AUTHOR";
    quotesHead->next = NULL;
    encryptedPuzzle = strdup("IFMMP XPSME");
}

void respond_with_file(int client_fd, const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        respond_not_found(client_fd);
        return;
    }

    struct stat st;
    fstat(fd, &st);

    char header[BUFFER_SIZE];
    snprintf(header, sizeof(header), RESPONSE_HEADER_TEMPLATE, OK_STATUS, st.st_size);
    write(client_fd, header, strlen(header));

    char file_buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, file_buffer, sizeof(file_buffer))) > 0) {
        write(client_fd, file_buffer, bytes_read);
    }

    close(fd);
}

void respond_not_found(int client_fd) {
    write(client_fd, NOT_FOUND_RESPONSE, strlen(NOT_FOUND_RESPONSE));
}