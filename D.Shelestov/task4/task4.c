#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

#define BUFFER_SIZE 256

typedef struct Node_s {
    char *string;
    struct Node_s *next;
} Node;

Node *head, *tail;

void init() {
    head = malloc(sizeof(Node));
    head->string = NULL;
    head->next = NULL;
    tail = head;
}

void push(char *string) {
    unsigned long len = strlen(string) + 1;
    char *copyPtr = calloc(len, sizeof(char));
    strcpy(copyPtr, string);

    tail->string = copyPtr;
    tail->next = calloc(1, sizeof(Node));
    tail = tail->next;
}

void printList() {
    Node *ptr = head;
    while (ptr != NULL) {
        if (ptr->string) {
            printf("%s\n", ptr->string);
        }
        ptr = ptr->next;
    }
}

// Function to set terminal raw mode
void setRawMode(int enable) {
    static struct termios original;
    static int restored = 1;
    
    if (enable) {
        if (restored) {
            tcgetattr(STDIN_FILENO, &original);
            restored = 0;
        }
        
        struct termios raw = original;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    } else {
        if (!restored) {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
            restored = 1;
        }
    }
}

// Function to read characters with filtering
int readFilteredLine(char *buffer, int size) {
    int pos = 0;
    char c;
    
    setRawMode(1);
    
    while (1) {
        if (read(STDIN_FILENO, &c, 1) != 1) {
            continue;
        }
        
        // Handle escape sequences (arrow keys, etc.)
        if (c == 27) { // ESC character
            // Read the next character to determine if it's an arrow key
            char next_chars[2];
            if (read(STDIN_FILENO, &next_chars[0], 1) == 1) {
                if (next_chars[0] == '[') {
                    // Read the third character
                    if (read(STDIN_FILENO, &next_chars[1], 1) == 1) {
                        // Arrow keys: A=up, B=down, C=right, D=left
                        // Function keys: 1~6, etc.
                        // Just ignore all escape sequences
                        continue;
                    }
                }
            }
            continue;
        }
        
        // Handle control characters
        if (c == '\n' || c == '\r') { // Enter
            printf("\n");
            break;
        } else if (c == 127 || c == 8) { // Backspace
            if (pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
        } else if (c == 3) { // Ctrl+C
            setRawMode(0);
            exit(0);
        } else if (c == 4) { // Ctrl+D
            setRawMode(0);
            return -1;
        } else if (c == '.') { // Dot for termination
            if (pos == 0) {
                buffer[pos++] = c;
                printf("%c", c);
                fflush(stdout);
                printf("\n");
                break;
            }
        } else if (isalpha((unsigned char)c) || c == ' ') { // Letters and spaces
            if (pos < size - 1) {
                buffer[pos++] = c;
                printf("%c", c);
                fflush(stdout);
            }
        }
        // All other characters (digits, special characters) are ignored
    }
    
    buffer[pos] = '\0';
    setRawMode(0);
    return pos;
}

int main() {
    char inputBuf[BUFFER_SIZE] = {0};

    init();
    printf("Enter strings (letters and spaces only). Enter '.' to print results.\n");

    while (1) {
        printf("Enter string: ");
        fflush(stdout);
        
        int length = readFilteredLine(inputBuf, BUFFER_SIZE);
        
        if (length == -1) { // Ctrl+D
            printf("\nProgram terminated.\n");
            break;
        }
        
        if (inputBuf[0] == '.' && length == 1) {
            printf("Result:\n");
            printList();
            return 0;
        }
        
        if (length > 0) {
            push(inputBuf);
        } else {
            printf("Empty string not added.\n");
        }
    }

    return 0;
}
