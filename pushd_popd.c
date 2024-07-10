#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

typedef struct Node {
    char dir[PATH_MAX];
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

void initStack(Stack* stack) {
    stack->top = NULL;
}

void pushd(Stack* stack, const char* dir) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->dir, dir, PATH_MAX);
    newNode->next = stack->top;
    stack->top = newNode;
}

char* popd(Stack* stack) {
    if (stack->top == NULL) {
        fprintf(stderr, "Stack is empty\n");
        return NULL;
    }
    Node* temp = stack->top;
    stack->top = stack->top->next;
    char* dir = strdup(temp->dir);
    free(temp);
    return dir;
}


char* getcwd_str() {
    char* buffer = (char*)malloc(PATH_MAX);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    if (getcwd(buffer, PATH_MAX) == NULL) {
        perror("getcwd");
        free(buffer);
        exit(EXIT_FAILURE);
}
    return buffer;
}

int main() {
    Stack dirStack;
    initStack(&dirStack);
    
    char* currentDir = getcwd_str();
    printf("Current Directory: %s\n", currentDir);

    pushd(&dirStack, currentDir);
    free(currentDir);

    if (chdir("/tmp") == 0) {
        currentDir = getcwd_str();
        printf("Changed to Directory: %s\n", currentDir);

       
        pushd(&dirStack, currentDir);
        free(currentDir);
    } else {
        perror("chdir");
    }
    char* previousDir = popd(&dirStack);
    if (previousDir != NULL) {
        printf("Popped Directory: %s\n", previousDir);
        if (chdir(previousDir) == 0) {
            currentDir = getcwd_str();
            printf("Changed back to Directory: %s\n", currentDir);
            free(currentDir);
        } else {
            perror("chdir");
        }
        free(previousDir);
    }
    while ((previousDir = popd(&dirStack)) != NULL) {
        free(previousDir);
    }

    return 0;
}
