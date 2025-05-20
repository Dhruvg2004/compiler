#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 
#include <libgen.h>
#include <assert.h>

#include "lexerf.h"
#include "parserf.h"
#include "codegeneratorf.h"
// #include "hashmap/hashmap.h"

// const unsigned initial_size = 100;
// struct hashmap_s hashmap;

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

    // assert(hashmap_create(initial_size, &hashmap) == 0 && "ERROR: Could not create hashmap\n");
    // hashmap_put(&hashmap,"dhruv",5,"gupta");
    Node *root = parser(tokens);
    // Node *root = parser(tokens,&hashmap);
    // int val = hashmap_num_entries(&hashmap);
    // printf("vadddddddddlue = %d\n",val);

    // char* hehe = (char*)hashmap_get(&hashmap,"hello",5);
    // printf("vadddddd = %s\n",hehe);
    generate_code(root);
    printf("\nOutput of the code:\n");
    system("./buildasm.sh");
    fclose(file);
    return 0;
}   