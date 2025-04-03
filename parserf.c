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

void print_error(char *error_type)
{
    printf("ERROR: %s\n", error_type);
    exit(1);
}



Token *generate_operation_nodes(Token *current_token, Node *current_node)
{

    Node *oper_node = current_node;    
    current_token--;
    if(current_token->type == INT){
        Node *expr_node = malloc(sizeof(Node));
        expr_node = init_node(expr_node, current_token->value, INT);
        oper_node->left = expr_node;
        current_token++;
        current_token++;
        if(current_token->type == INT)
        {
            current_token++;
            if(current_token->type == OPERATOR)
            {
                Node* nxtnode=malloc(sizeof(Node));
                oper_node->right=init_node(nxtnode,current_token->value,OPERATOR);
                current_token = generate_operation_nodes(current_token, oper_node->right);
                // current_token--;
            } 
            else 
            {
                current_token--;
                Node *expr_node = malloc(sizeof(Node));
                expr_node = init_node(expr_node, current_token->value, INT);
                oper_node->right = expr_node;
            }
        }
        else
        {
            print_error("ERROR: Missing value after operator");
        }
    }
    else
    {
        print_error("ERROR: Missing value before operator");
    }
    return current_token;
}
  


// Token *generate_operation_nodes(Token *current_token, Node *current_node)
// {
//     int iteration;
//     while(current_token->type == INT || current_token->type == OPERATOR){
//         iteration++;
//         Node *oper_node = malloc(sizeof(Node));
//         oper_node = init_node(oper_node, current_token->value, OPERATOR);
//         current_node->left = oper_node;
//         printf("CURRENT TOKEN 1: %s\n", current_token->value);
//         current_token--;
//         if(current_token->type == INT){
//           Node *expr_node = malloc(sizeof(Node));
//           expr_node = init_node(expr_node, current_token->value, INT);
//           oper_node->left = expr_node;
//           printf("CURRENT TOKEN 2: %s\n", current_token->value);
//           current_token++;
//           current_token++;
//           printf("CURRENT TOKEN 3: %s\n", current_token->value);
//           if(current_token->type != INT || current_token == NULL){
//             printf("Syntax Error hERE\n");
//             exit(1);
//           }
//           Node *second_expr_node = malloc(sizeof(Node));
//           second_expr_node = init_node(second_expr_node, current_token->value, INT);
//           oper_node->right = second_expr_node;
//         }
//         if(current_token->type == OPERATOR){
//          // 
//         }
//         current_token++;
//       }
//       return current_token;
// }

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
                            current_token++;
                            if(current_token->type == OPERATOR)
                            {
                                Node* nxtnode=malloc(sizeof(Node));
                                open_paren_node->left=init_node(nxtnode,current_token->value,OPERATOR);              
                                current_token = generate_operation_nodes(current_token, open_paren_node->left);
                                printf("current token: %s\n", current_token->value);
                            } 
                            else 
                            {
                              current_token--;
                              Node *expr_node = malloc(sizeof(Node));
                              expr_node = init_node(expr_node, current_token->value, INT);
                              open_paren_node->left = expr_node;
                            }
                            current_token++;
                            printf("current token: %s\n", current_token->value);
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
                                    print_error("ERROR : Invalid syntax semi");
                                }
                            }
                            else
                            {
                                print_error("ERROR : Invalid syntax on close");
                            }
                        }
                        else
                        {
                            print_error("ERROR : Invalid syntax int");
                        }

                    }
                    else
                    {
                        print_error("ERROR : Invalid syntaxon open");       
                    }
                }
                else
                {
                    print_error("ERROR : Invalid syntax on exit");
                }
            case SEPARATOR:
                break;
            case INT:
                printf("int\n");
                break;
            case OPERATOR:
                printf("op");
                break;
            case BEGINNING:
            case END_OF_TOKENS:
        }
        current_token++;
    }
    print_tree(root,0,"root");
    return root;
}