#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LENGTH 100

typedef enum {
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

char* expr;
int pos = 0;

Token getNextToken() {
    Token token;
    char* start = expr + pos;
    
    while (isspace(expr[pos])) pos++;
    
    if (expr[pos] == '\0') {
        token.type = TOKEN_END;
        token.value = NULL;
    } else if (strncmp(start, "TRUE", 4) == 0) {
        token.type = TOKEN_TRUE;
        token.value = "TRUE";
        pos += 4;
    } else if (strncmp(start, "FALSE", 5) == 0) {
        token.type = TOKEN_FALSE;
        token.value = "FALSE";
        pos += 5;
    } else if (strncmp(start, "AND", 3) == 0) {
        token.type = TOKEN_AND;
        token.value = "AND";
        pos += 3;
    } else if (strncmp(start, "OR", 2) == 0) {
        token.type = TOKEN_OR;
        token.value = "OR";
        pos += 2;
    } else if (strncmp(start, "NOT", 3) == 0) {
        token.type = TOKEN_NOT;
        token.value = "NOT";
        pos += 3;
    } else if (expr[pos] == '(') {
        token.type = TOKEN_LPAREN;
        token.value = "(";
        pos++;
    } else if (expr[pos] == ')') {
        token.type = TOKEN_RPAREN;
        token.value = ")";
        pos++;
    } else {
        printf("Unexpected character: %c\n", expr[pos]);
        exit(1);
    }
    
    return token;
}

int parse_expression();

int parse_factor() {
    Token token = getNextToken();
    if (token.type == TOKEN_TRUE) return 1;
    if (token.type == TOKEN_FALSE) return 0;
    if (token.type == TOKEN_NOT) return !parse_factor();
    if (token.type == TOKEN_LPAREN) {
        int result = parse_expression();
        if (getNextToken().type != TOKEN_RPAREN) {
            printf("Expected closing parenthesis\n");
            exit(1);
        }
        return result;
    }
    printf("Unexpected token\n");
    exit(1);
}

int parse_term() {
    int result = parse_factor();
    Token token = getNextToken();
    while (token.type == TOKEN_AND) {
        result = result && parse_factor();
        token = getNextToken();
    }
    pos -= strlen(token.value);
    return result;
}

int parse_expression() {
    int result = parse_term();
    Token token = getNextToken();
    while (token.type == TOKEN_OR) {
        result = result || parse_term();
        token = getNextToken();
    }
    pos -= strlen(token.value);
    return result;
}

int evaluate_expression(char* expression) {
    expr = expression;
    pos = 0;
    return parse_expression();
}

int main() {
    char input[MAX_EXPR_LENGTH];
    printf("Enter a boolean expression (use TRUE, FALSE, AND, OR, NOT, and parentheses):\n");
    fgets(input, MAX_EXPR_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline

    int result = evaluate_expression(input);
    printf("Result: %s\n", result ? "TRUE" : "FALSE");

    return 0;
}