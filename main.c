#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 
#include <libgen.h>

#include "lexerf.h"
#include "parserf.h"
#include "codegeneratorf.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Error : correct syntax : ./%s <filename.sped>\n",basename(argv[0]));
        exit(1);
    }
    FILE* file;
    file = (fopen(argv[1],"r"));
    if(!file)
    {
        printf("Error : File not found\n");
        exit(1);
    }
    Token *tokens = lexer(file);
    for(size_t i = 0 ; tokens[i].type != END_OF_TOKENS; i++)
    {
        print_token(tokens[i]);
    }
    
    Node *root = parser(tokens);
    generate_code(root);
    system("./buildasm.sh");
    fclose(file);
    return 0;
}   