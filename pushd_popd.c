#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#define STACK_SIZE 10

typedef struct {
    char dirs[STACK_SIZE][PATH_MAX];
    int top;
} Stack;

void initStack(Stack* stack) {
    stack->top = -1;
}

void pushd(Stack* stack, const char* dir) {
    if (stack->top >= STACK_SIZE - 1) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    stack->top++;
    strncpy(stack->dirs[stack->top], dir, PATH_MAX);
}

char* popd(Stack* stack) {
    if (stack->top < 0) {
        fprintf(stderr, "Stack underflow\n");
        return NULL;
    }
    return stack->dirs[stack->top--];
}

int main() {
    Stack dirStack;
    initStack(&dirStack);

    char currentDir[PATH_MAX];
    if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("Current Directory: %s\n", currentDir);
    
    pushd(&dirStack, currentDir);

    if (chdir("/tmp") == 0) {
        if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
            perror("getcwd");
            return 1;
        }
        printf("Changed to Directory: %s\n", currentDir);

        pushd(&dirStack, currentDir);
    } else {
        perror("chdir");
    }
    
    char* previousDir = popd(&dirStack);
    if (previousDir != NULL) {
        printf("Popped Directory: %s\n", previousDir);
        if (chdir(previousDir) == 0) {
            if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
                perror("getcwd");
                return 1;
            }
            printf("Changed back to Directory: %s\n", currentDir);
        } else {
            perror("chdir");
        }
    }

    return 0;
}

