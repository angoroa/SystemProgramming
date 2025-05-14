#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

void infix_to_postfix(char* infix, char* postfix) {
    char opStack[100];
    int top = -1, j = 0;
    char* token = strtok(infix, " ");

    while (token != NULL) {
        if (isdigit(token[0])) {
            int k = 0;
            while (token[k] != '\0') {
                postfix[j++] = token[k++];
            }
            postfix[j++] = ' ';
        } else if (strchr("+-*/", token[0])) {
            while (top >= 0 && precedence(opStack[top]) >= precedence(token[0])) {
                postfix[j++] = opStack[top--];
                postfix[j++] = ' ';
            }
            opStack[++top] = token[0];
        }
        token = strtok(NULL, " ");
    }

    while (top >= 0) {
        postfix[j++] = opStack[top--];
        postfix[j++] = ' ';
    }
    postfix[j] = '\0';
}

int eval_postfix(char* postfix) {
    int stack[100];
    int top = -1;
    char* token = strtok(postfix, " ");

    while (token != NULL) {
        if (isdigit(token[0])) {
            stack[++top] = atoi(token);
        } else if (strchr("+-*/", token[0])) {
            int b = stack[top--];
            int a = stack[top--];
            switch (token[0]) {
                case '+': stack[++top] = a + b; break;
                case '-': stack[++top] = a - b; break;
                case '*': stack[++top] = a * b; break;
                case '/': stack[++top] = a / b; break;
            }
        }
        token = strtok(NULL, " ");
    }
    return stack[top];
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char infix[100], postfix[100];
    fgets(infix, sizeof(infix), fp);
    fclose(fp);
    infix[strcspn(infix, "\n")] = '\0';

    infix_to_postfix(infix, postfix);
    int result = eval_postfix(postfix);

    printf("결과: %d\n", result);
    return 0;
}

