#ifndef LEXER_H_
#define LEXER_H_

typedef enum{
    BEGINNING,
    INT,
    KEYWORD,
    OPERATOR,
    SEPARATOR,
    END_OF_TOKENS,
} TokenType;

typedef struct{
    TokenType type;
    char *value;
} Token;

void print_token(Token token);
Token *generate_number(char current,FILE *file);
Token *generate_keyword(char current,FILE *file);
Token *generate_separator_or_operator(char current, TokenType type);
Token *lexer (FILE *file);

#endif