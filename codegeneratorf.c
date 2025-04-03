#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "lexerf.h"
#include "parserf.h"

void arithmetic(Node *node,FILE *file)
{
    if(node->right->type == OPERATOR)
    {
        if(strcmp(node->value,"+") == 0)
        {
            fprintf(file, "\tadd rdi, %s\n", node->right->left->value);
        }
        if(strcmp(node->value,"-") == 0)
        {
            fprintf(file, "\tsub rdi, %s\n", node->right->left->value);
        }
        arithmetic(node->right,file);
    }
    else
    {
        if(strcmp(node->value,"+") == 0)
        {
            fprintf(file, "\tadd rdi, %s\n", node->right->value);
        }
        if(strcmp(node->value,"-") == 0)
        {
            fprintf(file, "\tsub rdi, %s\n", node->right->value);
        }
    }
}

void traverse_tree(Node *node, int is_left, FILE *file)
{
    if(node == NULL)
    {
        return;
    }
    else if(strcmp(node->value, "exit") == 0)
    {
        fprintf(file, "\tmov rax, 60\n");
    }
    else if(strcmp(node->value, "(") == 0)
    {

    }
    else if(node->type == OPERATOR)
    {
        fprintf(file, "\tmov rdi, %s\n", node->left->value);
        arithmetic(node,file);
        return;
    }
    else if(node->type == INT)
    {
        fprintf(file, "\tmov rdi, %s\n", node->value);
    }
    else if(strcmp(node->value, ")") == 0)
    {

    }
    else if(strcmp(node->value, ";") == 0)
    {
        fprintf(file, "\tsyscall\n");
    }
    else if(is_left)
    {

    }
    else
    {

    }
    for(size_t i = 0; node->value[i] != '\0'; i++)
    {
        printf("%c", node->value[i]);
    }
    printf("\n");
    traverse_tree(node->left, 1, file);
    traverse_tree(node->right, 0, file);
}

int generate_code(Node *root)
{
    FILE *file = fopen("generate.asm", "w");
    assert(file != NULL && "FILE COULD NOT BE OPENED\n");

    fprintf(file, "section .data\n");
    fprintf(file, "\tprintf_format: db '%s', 10, 0\n", "%d");
    fprintf(file, "section .text\n");
    fprintf(file, "\tglobal main\n");
    fprintf(file, "\textern printf\n");
    fprintf(file, "main:\n");
    // fprintf(file, "\tmov rdi, 0\n");
    //print_tree(root, 0, "root");

    traverse_tree(root, 0, file);
    // system("./buildasm.sh");
    // system("./generate");
    fclose(file);

    return 0;
}