#ifndef LEXER_H_
#define LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

typedef enum{
    INT,
    KEYWORD,
    SEPARATOR,
    END_OF_TOKENS
} TokenType;

typedef struct{
    TokenType type;
    char *value;
} Token;

void print_token(Token token);
Token *generate_number(char current,FILE *file);
Token *generate_keyword(char current,FILE *file);
Token *lexer (FILE *file);

#endif