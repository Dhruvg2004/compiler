#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexerf.h"

typedef struct Node {
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
} Node;

void print_tree(Node *node)
{
    if(node == NULL)
    {
        return;
    }
    for(size_t i = 0; node->value[i] != '\0'; i++)
    {
        printf("%c", node->value[i]);
    }
    printf("\n");
    print_tree(node->left);
    print_tree(node->right);
}

Node *init_node(Node *node, char *value, TokenType type)
{
    node = malloc(sizeof(Node));
    node->value = malloc(sizeof(char) * 2);
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Token *parser(Token *tokens)
{
    Token *current_token = &tokens[0];
    Node *root = malloc(sizeof(Node));
    Node *left = malloc(sizeof(Node));
    Node *right = malloc(sizeof(Node));
    root = init_node(root, "PROGRAM", BEGINNING);

    // print_tree(root);

    Node *current = root;

    while(current_token->type != END_OF_TOKENS)
    {
        if(current == NULL)
        {
            break;
        }
        if(current == root)
        {
            //;
        }
        

        if(current_token->type == KEYWORD && strcmp(current_token->value, "exit"))
        {
            Node *exit_node = malloc(sizeof(Node));
            exit_node = init_node(exit_node, current_token->value, KEYWORD);
            root->right = exit_node; 
            current = exit_node;
            current_token++;
            if(current_token->type != SEPARATOR)
            {
                printf("ERROR\n");
                exit(1);
            }
            Node *open_paren_node = malloc(sizeof(Node));
            open_paren_node = init_node(open_paren_node, current_token->value, SEPARATOR);
            current->left = open_paren_node;

            current_token++;

            Node *expr_node = malloc(sizeof(Node));
            expr_node = init_node(expr_node, current_token->value, INT);
            current->left->left = expr_node;

            current_token++;

            Node *close_paren_node = malloc(sizeof(Node));
            close_paren_node = init_node(close_paren_node, current_token->value, SEPARATOR);
            current->left->right = close_paren_node;

            current_token++;

            Node *semi_node = malloc(sizeof(Node));
            semi_node = init_node(semi_node, current_token->value, SEPARATOR);
            current->right = semi_node;
            
            printf("EXIT\n");
            break;
        }
        current_token++;
    }
    print_tree(root);
    return current_token;
}