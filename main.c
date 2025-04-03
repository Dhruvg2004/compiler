#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum{
    SEMI,
    O_PARA,
    C_PARA,
} TypeSeparator;


typedef enum{
    EXIT,
} TypeKeyword;


typedef enum{
    INT,
} TypeLiteral;


typedef struct{
    TypeSeparator type;
} TokenSeparator;


typedef struct{
    TypeKeyword type;
    char *value;
} TokenKeyword;


typedef struct{
    TypeLiteral type;
    char *value;
} TokenLiteral;

TokenLiteral *generate_number(char current,FILE *file)
{
    TokenLiteral *token = malloc(sizeof(TokenLiteral));
    token->type=INT;
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
    return (token);
}

TokenKeyword *generate_keyword(char current,FILE *file)
{
    TokenKeyword *token = malloc(sizeof(TokenKeyword));
    token->type=INT;
    char *keyword = malloc(sizeof(char) * 8 );
    int keyword_index=0;
    while(isalpha(current) && current !=EOF)
    {
        keyword[keyword_index] = current;
        keyword_index++;
        current = fgetc(file); 
    }
    ungetc(current, file);
    keyword[keyword_index]='\0';
    // printf("infunction : %s\n", keyword);
    if(!strcmp(keyword,"exit"))
    {
        token->type = EXIT;
        token->value=keyword;
    }
    
    return (token);
}

void lexer (FILE *file)
{
    char current = fgetc(file);
    while(current != EOF){
        if(current == ';')
        {
            printf("found semicolon\n");
        }
        else if(current == '(')
        {
            printf("found open para\n");
        }
        else if(current == ')')
        {
            printf("found close para\n");
        }
        else if(isdigit(current)){
            TokenLiteral *test = generate_number(current,file);
            printf("Number is : %s\n",test->value);
        }
        else if(isalpha(current)){
            TokenKeyword *test = generate_keyword(current,file);
            printf("string is : %s\n",test->value);
        }
        current = fgetc(file);
    }
}


int main()
{
    FILE* file;
    file = (fopen("test.sped","r"));
    lexer(file);
    // printf("\n");
    return 0;
}   