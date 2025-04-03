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

void print_tree(Node *node, int indent, char *identifier)
{
    if(node == NULL){
      return;
    }
    for(int i = 0; i < indent; i++){
      printf(" ");
    }
    printf("%s -> ", identifier);
    for(size_t i = 0; node->value[i] != '\0'; i++){
      printf("%c", node->value[i]);
    }
    printf("\n");
    print_tree(node->left, indent + 1, "left");
    print_tree(node->right, indent + 1, "right");
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

Node *parser(Token *tokens)
{
    Token *current_token = &tokens[0];
    Node *root = malloc(sizeof(Node));
    root = init_node(root, "PROGRAM", BEGINNING);


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
        switch(current_token->type)
        {
            case KEYWORD:
                if(current_token->type == KEYWORD && !strcmp(current_token->value, "exit"))
                {
                    Node *exit_node = malloc(sizeof(Node));
                    exit_node = init_node(exit_node, current_token->value, KEYWORD);
                    root->right = exit_node;
                    current = exit_node;
                    current_token++;
                    if(current_token->type == SEPARATOR && !strcmp(current_token->value , "(") )
                    {
                        Node *open_paren_node = malloc(sizeof(Node));
                        open_paren_node = init_node(open_paren_node, current_token->value, SEPARATOR);
                        current->left = open_paren_node;
                        // current = open_paren_node;
                        current_token++;
                        if(current_token->type == INT)
                        {
                            Node *expr_node = malloc(sizeof(Node));
                            expr_node = init_node(expr_node, current_token->value, INT);
                            current->left->left = expr_node;
                            current_token++;
                            if(current_token->type == SEPARATOR && !strcmp(current_token->value , ")"))
                            {
                                Node *close_paren_node = malloc(sizeof(Node));
                                close_paren_node = init_node(close_paren_node, current_token->value, SEPARATOR);
                                current->left->right = close_paren_node;
                                current_token++;
                                if( current_token->type == SEPARATOR && !strcmp(current_token->value , ";"))
                                {
                                    Node *semi_node = malloc(sizeof(Node));
                                    semi_node = init_node(semi_node, current_token->value, SEPARATOR);
                                    current->right = semi_node;
                                    break;
                                }
                                else
                                {
                                    printf("ERROR : Invalid syntax semi\n");
                                    exit(1);
                                }
                            }
                            else
                            {
                                printf("ERROR : Invalid syntax on close\n");
                                exit(1);
                            }
                        }
                        else
                        {
                            printf("ERROR : Invalid syntax int\n");
                            exit(1);
                        }

                    }
                    else
                    {
                        printf("ERROR : Invalid syntaxon open\n");
                        exit(1);        
                    }
                }
                else
                {
                    printf("ERROR : Invalid syntax on exit -> %s\n",current_token->value);
                    exit(1);
                }
            case SEPARATOR:
                break;
            case INT:
                printf("int\n");
                break;
            case BEGINNING:
            case END_OF_TOKENS:
        }
        current_token++;
    }
    print_tree(root,0,"root");
    return root;
}