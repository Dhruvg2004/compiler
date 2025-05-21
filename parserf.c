#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "lexerf.h"
#include "hashmap/hashmap.h"

const unsigned initial_size2 = 10000;
struct hashmap_s hashmap;




typedef struct Scope {
    hashmap_t table;
    struct Scope *parent;
    int level;
} Scope;

// Create a new scope
Scope *enter_scope(Scope *current) {
    Scope *new_scope = malloc(sizeof(Scope));
    assert(hashmap_create(initial_size2, &new_scope->table) == 0 && "ERROR: Could not create hashmap\n");
    new_scope->parent = current;
    new_scope->level = current ? current->level + 1 : 0;
    return new_scope;
}

// Exit current scope
Scope *exit_scope(Scope *current) {
    hashmap_destroy(&current->table);
    Scope *parent = current->parent;
    free(current);
    return parent;
}

// Insert variable into current scope
void insert_symbol(Scope *scope, const char *name, char *sym) {
    hashmap_put(&scope->table, name, strlen(name), sym);
}

// Lookup variable up the scope chain
char *lookup(Scope *scope, const char *name) {
    while (scope) {
        char *sym = hashmap_get(&scope->table, name, strlen(name));
        if (sym) return sym;
        scope = scope->parent;
    }
    return NULL;
}

void update_symbol_in_all_scopes(Scope *scope, const char *name, char* change) {
    while (scope) {
        char *sym = hashmap_get(&scope->table, name, strlen(name));
        if (sym) {
            hashmap_remove(&scope->table, (name), strlen((name)));
            hashmap_put(&scope->table, (name), strlen((name)),change);
            return;
        }
        scope = scope->parent;
    }
}

Scope *global = NULL;









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


void handle_token_errors(char *error_text, Token *current_token, TokenType type){
  if(current_token->type == END_OF_TOKENS || current_token->type != type){
    print_error(error_text);
  }
}




double evaluate(Token **current_token);
double parseExpression(Token **current_token);
double parseTerm(Token **current_token);
double parseFactor(Token **current_token);







double evaluate(Token **current_token) {
    return parseExpression(current_token);
}

double parseExpression(Token **current_token) {
    double result = parseTerm(current_token);
    
    while ((*current_token)->type == OPERATOR &&
           (!strcmp((*current_token)->value,"+")|| !strcmp((*current_token)->value,"-"))) {
        char op = (*current_token)->value[0];
        (*current_token)++;  // Move to next token
        double term = parseTerm(current_token);
        if (op == '+') result += term;
        else result -= term;
    }
    return result;
}

double parseTerm(Token **current_token) {
    double result = parseFactor(current_token);
    
    while ((*current_token)->type == OPERATOR &&
           (!strcmp((*current_token)->value,"*") || !strcmp((*current_token)->value,"/"))) {
        char op = (*current_token)->value[0];
        (*current_token)++;
        double factor = parseFactor(current_token);
        if (op == '*') result *= factor;
        else if (factor != 0) result /= factor;
        else {
            printf("Error: Division by zero\n");
            exit(1);
        }
    }
    return result;
}

double parseFactor(Token **current_token) 
{
    double result = 0;

    if ((*current_token)->type == SEPARATOR && !strcmp((*current_token)->value,"(")) 
    {
        (*current_token)++;  // Skip '('
        result = parseExpression(current_token);
        if ((*current_token)->type == SEPARATOR && !strcmp((*current_token)->value,")")) 
        {
            (*current_token)++;  // Skip ')'
        } else {
            printf("Error: Mismatched parentheses\n");
            exit(1);
        }
    } 
    else if ((*current_token)->type == INT) 
    {
        result = strtod((*current_token)->value, NULL);  // Convert token value to double
        (*current_token)++;  // Move to the next token
    } 
    else if((*current_token)->type == IDENTIFIER){
        //if the identifier arrives it will auto solve it self
        char* val=hashmap_get(&global->table, (*current_token)->value, strlen((*current_token)->value));
        if(val== 0)
        {
            printf("ERROR: \"%s\" does not exist in hash table!\n",(*current_token)->value);
            exit(1);
        }

        result = strtod(val, NULL); 
        (*current_token)++;
    }
    else 
    {
        printf("Error: Invalid/Missing value in expression %s\n",(*current_token)->value);
        exit(1);
    }

    return result;
}














// Token *generate_operation_nodes(Token *current_token, Node *current_node)
// {
//     Node *oper_node = current_node;    
//     current_token--;
//     if(current_token->type == INT){
//         Node *expr_node = malloc(sizeof(Node));
//         expr_node = init_node(expr_node, current_token->value, INT);
//         oper_node->left = expr_node;
//         current_token++;
//         current_token++;
//         if(current_token->type == INT)
//         {
//             current_token++;
//             if(current_token->type == OPERATOR)
//             {
//                 Node* nxtnode=malloc(sizeof(Node));
//                 oper_node->right=init_node(nxtnode,current_token->value,OPERATOR);
//                 current_token = generate_operation_nodes(current_token, oper_node->right);
//                 // current_token--;
//             } 
//             else 
//             {
//                 current_token--;
//                 Node *expr_node = malloc(sizeof(Node));
//                 expr_node = init_node(expr_node, current_token->value, INT);
//                 oper_node->right = expr_node;
//             }
//         }
//         else
//         {
//             print_error("ERROR: Missing value after operator");
//         }
//     }
//     else
//     {
//         print_error("ERROR: Missing value before operator");
//     }
//     return current_token;
// }
  


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


Node* handle_exit(Node *root, Token **current_t, Node *current)
{
    Token *current_token=*current_t;
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
        current_token++;
        int res=(int)evaluate(&current_token);
        char *val=malloc(sizeof(char) * 8 );
        sprintf(val,"%d",res);

        Node* nxtnode=malloc(sizeof(Node));
        open_paren_node->left=init_node(nxtnode,val,INT);    
        // printf("current token: %s\n", current_token->value);
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
        print_error("ERROR : Invalid syntaxon open");       
    }
    *current_t=current_token;
    return current;
}


Node* create_variable(Token **current_t, Node *current)
{
    // printf("correct1");
    Token *current_token=*current_t;
    Node *var_node = malloc(sizeof(Node));
    var_node = init_node(var_node, current_token->value, KEYWORD);
    current->left = var_node;
    current = var_node;
    current_token++;
    // printf("correct2");
    handle_token_errors("Invalid syntax after INT", current_token, IDENTIFIER);
    // printf("correct3");
    if(current_token->type == IDENTIFIER){
        Node *identifier_node = malloc(sizeof(Node));
        identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
        current->left = identifier_node;
        current = identifier_node;
        current_token++;
    }
    // printf("correct4");
    handle_token_errors("Invalid Syntax After Identifier", current_token, OPERATOR);
    // printf("correct5");
    if(current_token->type == OPERATOR){
        if(strcmp(current_token->value, "=") != 0){
            print_error("Invalid Variable Syntax on =");
        }
        Node *equals_node = malloc(sizeof(Node));
        equals_node = init_node(equals_node, current_token->value, OPERATOR);
        current->left = equals_node;
        current = equals_node;
        current_token++;
    }
    // printf("correct6");
    if(current_token->type == END_OF_TOKENS || (current_token->type != INT && current_token->type != IDENTIFIER && strcmp(current_token->value , "(") ))
    {
        print_error("Invalid Syntax After Equals");
    }

    int res=(int)evaluate(&current_token);
    char *val=malloc(sizeof(char) * 8 );
    sprintf(val,"%d",res);

    Node *expr_node = malloc(sizeof(Node));
    expr_node = init_node(expr_node, val, INT);
    current->left = expr_node;
    // current_token++;
    // printf("correct7");
    if(hashmap_get(&global->table,var_node->left->value,strlen(var_node->left->value)) ){
        printf("ERROR: Reinitialising the variable in same scope!\n");
        exit(1);
    }

    
    // printf("correct8");   

    if(current_token->type == END_OF_TOKENS || (current_token->type != SEPARATOR || strcmp(current_token->value , ";") ))
    {
        print_error("Invalid Syntax MISSING SEMICOLON");
    }
    // printf("correct9");/
    current = var_node;
    if(strcmp(current_token->value, ";") == 0)
    {
        Node *semi_node = malloc(sizeof(Node));
        semi_node = init_node(semi_node, current_token->value, SEPARATOR);
        current->right = semi_node;
        current = semi_node;
    }
    // else
    // exit(1);
    if(hashmap_put(&global->table,var_node->left->value, strlen(var_node->left->value), val) != 0){
      printf("ERROR: Could not insert into hash table!\n");
      exit(1);
    }


    *current_t=current_token;
    return current;
}

Node* reloc_var(Token **current_t, Node *current)
{
    Token *current_token=*current_t;
    
    Node *identifier_node = malloc(sizeof(Node));
    identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
    current->left = identifier_node;
    current = identifier_node;
    current_token++;
    handle_token_errors("Invalid Syntax After Identifier", current_token, OPERATOR);

    if(current_token->type == OPERATOR){
        if(strcmp(current_token->value, "=") != 0){
            print_error("Invalid Variable Syntax on =");
        }
        Node *equals_node = malloc(sizeof(Node));
        equals_node = init_node(equals_node, current_token->value, OPERATOR);
        current->left = equals_node;
        current = equals_node;
        current_token++;
    }
    if(current_token->type == END_OF_TOKENS || (current_token->type != INT && current_token->type != IDENTIFIER && strcmp(current_token->value , "(") ))
    {
        print_error("Invalid Syntax After Equals");
    }

    int res=(int)evaluate(&current_token);
    char *val=malloc(sizeof(char) * 8 );
    sprintf(val,"%d",res);

    Node *expr_node = malloc(sizeof(Node));
    expr_node = init_node(expr_node, val, INT);
    current->left = expr_node;
    // current_token++;

    


    if(current_token->type == END_OF_TOKENS || (current_token->type != SEPARATOR || strcmp(current_token->value , ";") ))
    {
        print_error("Invalid Syntax MISSING SEMICOLON");
    }

    current = identifier_node;
    if(strcmp(current_token->value, ";") == 0)
    {
        Node *semi_node = malloc(sizeof(Node));
        semi_node = init_node(semi_node, current_token->value, SEPARATOR);
        current->right = semi_node;
        current = semi_node;
    }

    if(hashmap_remove(&global->table, identifier_node->value, strlen(identifier_node->value)))
    {
        update_symbol_in_all_scopes(global,identifier_node->value,val);
    }
    else
    {
        if(hashmap_put(&global->table,identifier_node->value, strlen(identifier_node->value), val) != 0){
            printf("ERROR: Could not insert into hash table!\n");
            exit(1);
        }
    }


    


    *current_t=current_token;
    return current;
}


Node* handle_exit_scope( Token **current_t, Node *current)
{
    Token *current_token=*current_t;
    Node *exit_node = malloc(sizeof(Node));
    exit_node = init_node(exit_node, current_token->value, KEYWORD);
    current->left = exit_node;
    current = exit_node;
    current_token++;
    if(current_token->type == SEPARATOR && !strcmp(current_token->value , "(") )
    {
        Node *open_paren_node = malloc(sizeof(Node));
        open_paren_node = init_node(open_paren_node, current_token->value, SEPARATOR);
        current->left = open_paren_node;
        current_token++;
        int res=(int)evaluate(&current_token);
        char *val=malloc(sizeof(char) * 8 );
        sprintf(val,"%d",res);

        Node* nxtnode=malloc(sizeof(Node));
        open_paren_node->left=init_node(nxtnode,val,INT);    
        // printf("current token: %s\n", current_token->value);
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
        print_error("ERROR : Invalid syntaxon open");       
    }
    *current_t=current_token;
    return current;
}
Node *parser(Token *tokens)
{
    Token *current_token = &tokens[0];
    Node *root = malloc(sizeof(Node));
    root = init_node(root, "PROGRAM", BEGINNING);
    global = enter_scope(NULL);

    assert(hashmap_create(initial_size2, &hashmap) == 0 && "ERROR: Could not create hashmap\n");


    // hashmap_put(hashmap,"dhruv",5,"gupta");

    // int val = hashmap_num_entries(hashmap);
    // printf("value = %d\n",val);
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
                if(!strcmp(current_token->value, "exit"))
                {
                    printf("level: %d\n",global->level);
                    if(global->level)
                    {
                        current=handle_exit_scope(&current_token,current);
                    }
                    else
                    {
                        current=handle_exit(root,&current_token,current);
                    }
                    
                }
                else if(!strcmp(current_token->value, "int")){
                    current=create_variable(&current_token,current);
                }
                break;
            case SEPARATOR:
                if(!strcmp(current_token->value, "{"))
                {
                    global=enter_scope(global);
                }
                else if(!strcmp(current_token->value, "}"))
                {
                    global=exit_scope(global);
                }
                break;
            case INT:
                // printf("int\n");
                break;
            case OPERATOR:
                break;
            case IDENTIFIER:
                char* val=lookup(global,(current_token)->value);
                if(val== 0)
                {
                    printf("ERROR: \"%s\" is not initialised!\n",(current_token)->value);
                    exit(1);
                }
                else{
                    current=reloc_var(&current_token,current);
                }


                // printf("IDENTIFIER\n");
                break;
            case BEGINNING:
            case END_OF_TOKENS:
        }
        current_token++;
    }
    print_tree(root,0,"root");
    return root;
}