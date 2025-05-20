#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

typedef enum{
    BEGINNING,
    INT,
    KEYWORD,
    OPERATOR,
    IDENTIFIER,
    SEPARATOR,
    END_OF_TOKENS
} TokenType;

typedef struct{
    TokenType type;
    char *value;
} Token;

void print_token(Token token)
{
    printf("TOKEN VALUE : ");
    printf(" '");
    for(size_t i = 0;token.value[i] != '\0';i++)
    {
        printf("%c",token.value[i]);
    }
    printf("' ");
    switch(token.type)
    {
        case INT:
            printf("TOKEN TYPE : INT\n");
            break;
        case KEYWORD:
            printf("TOKEN TYPE : KEYWORD\n");
            break;
        case OPERATOR:
            printf("TOKEN TYPE : OPERATOR\n");
            break;
        case SEPARATOR:
            printf("TOKEN TYPE : SEPARATOR\n");
            break;
        case IDENTIFIER:
            printf("TOKEN TYPE : IDENTIFIER\n");
            break;
        case END_OF_TOKENS:
            printf("END OF TOKENS\n");
            break;
        case BEGINNING:
            printf("BEGINNING\n");
            break;
    }
}

Token *generate_number(char current,FILE *file)
{
    Token *token = malloc(sizeof(Token));
    char *value = malloc(sizeof(char) * 8 );
    int value_index=0;
    while(isdigit(current) && current !=EOF)
    {
        value[value_index] = current;
        value_index++;
        current = fgetc(file); 
    }
    ungetc(current, file);
    value[value_index]='\0';
    token->value = value;    
    token->type=INT;
    return (token);
}

Token *generate_keyword_or_identifier(char current,FILE *file)
{
    Token *token = malloc(sizeof(Token));
    char *keyword = malloc(sizeof(char) * 1024 );
    int keyword_index=0;
    while(isalpha(current) && current !=EOF)
    {
        keyword[keyword_index] = current;
        keyword_index++;
        current = fgetc(file); 
    }
    ungetc(current, file);
    keyword[keyword_index]='\0';
    token->value=keyword;
    
    if(strcmp(keyword,("exit")) == 0)
    {
        token->type=KEYWORD;
    }
    else if(strcmp(keyword,"int") == 0)
    {
        token->type=KEYWORD;
    }
    else
    {
        token->type=IDENTIFIER;
    }
        
    return (token);
}

Token *generate_separator_or_operator(char current, TokenType type)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->value = malloc(sizeof(char)*2);
    token->value[0]=current;
    token->value[1]='\0';
    return token;
}

Token *lexer (FILE *file)
{
    char current = fgetc(file);
    int number_of_tokens=12;
    Token *tokens = malloc(sizeof(Token)*number_of_tokens);
    int token_size=0;
    size_t token_index = 0;
    while(current != EOF){
        Token *token = malloc(sizeof(Token));
        token_size++;
        if(token_size > number_of_tokens){
            number_of_tokens *= 1.5;
            tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
        }
        if(current == '{' || current == '}')
        {
            token = generate_separator_or_operator(current,SEPARATOR);   
            tokens[token_index++] = *token;
        }
        if(current == ';')
        {
            token = generate_separator_or_operator(current,SEPARATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '(')
        {
            token = generate_separator_or_operator(current,SEPARATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == ')')
        {
            token = generate_separator_or_operator(current,SEPARATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '=')
        {
            token = generate_separator_or_operator(current,OPERATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '+')
        {
            token = generate_separator_or_operator(current,OPERATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '*')
        {
            token = generate_separator_or_operator(current,OPERATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '/')
        {
            token = generate_separator_or_operator(current,OPERATOR);   
            tokens[token_index++] = *token;
        }
        else if(current == '-')
        {
            token = generate_separator_or_operator(current,OPERATOR);   
            tokens[token_index++] = *token;
        }
        else if(isdigit(current)){
            token = generate_number(current,file);
            tokens[token_index++] = *token;
        }
        else if(isalpha(current)){
            token = generate_keyword_or_identifier(current,file);
            tokens[token_index++] = *token;
        }
        current = fgetc(file);
        free(token);
    }
    tokens[token_index].value=NULL;
    tokens[token_index].type=END_OF_TOKENS;
    return tokens;
}