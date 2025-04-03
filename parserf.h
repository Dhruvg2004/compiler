#ifndef PARSER_H_
#define PARSER_H_

typedef struct Node {
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
} Node;





void print_tree(Node *node, int indent, char *identifier);
void print_error(char *error_type);
double evaluate(Token **current_token);
double parseExpression(Token **current_token);
double parseTerm(Token **current_token);
double parseFactor(Token **current_token);
Node *init_node(Node *node, char *value, TokenType type);
Token *generate_operation_nodes(Token *current_token, Node *current_node);
Node *parser(Token *tokens);

#endif