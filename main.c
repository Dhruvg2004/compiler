#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 

#include "lexerf.h"

int main()
{
    FILE* file;
    file = (fopen("test.sped","r"));
    Token *tokens = lexer(file);
    for(size_t i = 0 ; tokens[i].type != END_OF_TOKENS; i++)
    {
        print_token(tokens[i]);
    }
    return 0;
}   