#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_type_of_token
{
    T_NUMBER,
    T_PLUS,
    T_MINUS,
    T_MULTIPLY,
    T_EOF
} t_type_of_token;

typedef struct s_token
{
    t_type_of_token type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_ast_node
{
    t_type_of_token type;
    int value;
    struct s_ast_node *left;
    struct s_ast_node *right;
} t_ast_node;

t_token *create_token(t_type_of_token type, char *value)
{
    t_token *token = (t_token *)malloc(sizeof(t_token));
    token->type = type;
    token->value = strdup(value); // strdup to duplicate string
    token->next = NULL;
    return token;
}

void add_token(t_token **head, t_token *token)
{
    if (*head == NULL)
    {
        *head = token;
    }
    else
    {
        t_token *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = token;
    }
}

void print_tokens(t_token *head)
{
    t_token *current = head;
    while (current != NULL)
    {
        printf("type: %d, value: %s\n", current->type, current->value);
        current = current->next;
    }
}

t_token *lexer(char *input)
{
    t_token *head = NULL;
    t_token *current = NULL;
    char *value = (char *)malloc(1);
    value[0] = '\0';
    int i = 0;
    while (input[i] != '\0')
    {
        if (isdigit(input[i]))
        {
            value = (char *)realloc(value, strlen(value) + 1);
            value[strlen(value)] = input[i];
            value[strlen(value) + 1] = '\0';
        }
        else
        {
            if (strlen(value) > 0)
            {
                t_token *token = create_token(T_NUMBER, value);
                add_token(&head, token);
                value = (char *)malloc(1);
                value[0] = '\0';
            }
            if (input[i] == '+')
            {
                t_token *token = create_token(T_PLUS, "+");
                add_token(&head, token);
            }
            else if (input[i] == '-')
            {
                t_token *token = create_token(T_MINUS, "-");
                add_token(&head, token);
            }
            else if (input[i] == '*')
            {
                t_token *token = create_token(T_MULTIPLY, "*");
                add_token(&head, token);
            }
        }
        i++;
    }
    if (strlen(value) > 0)
    {
        t_token *token = create_token(T_NUMBER, value);
        add_token(&head, token);
    }
    t_token *token = create_token(T_EOF, "EOF");
    add_token(&head, token);
    return head;
}

t_ast_node *create_node(t_type_of_token type, int value)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

t_ast_node *parse_expression(t_token **tokens);

t_ast_node *parse_number(t_token **tokens)
{
    t_token *current = *tokens;
    *tokens = (*tokens)->next;
    return create_node(T_NUMBER, atoi(current->value));
}

t_ast_node *parse_term(t_token **tokens)
{
    t_ast_node *left = parse_number(tokens);
    while ((*tokens)->type == T_MULTIPLY)
    {
        t_type_of_token op = (*tokens)->type;
        *tokens = (*tokens)->next;
        t_ast_node *right = parse_number(tokens);
        t_ast_node *node = create_node(op, 0);
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

t_ast_node *parse_expression(t_token **tokens)
{
    t_ast_node *left = parse_term(tokens);
    while ((*tokens)->type == T_PLUS || (*tokens)->type == T_MINUS)
    {
        t_type_of_token op = (*tokens)->type;
        *tokens = (*tokens)->next;
        t_ast_node *right = parse_term(tokens);
        t_ast_node *node = create_node(op, 0);
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

void free_ast(t_ast_node *root)
{
    if (root)
    {
        free_ast(root->left);
        free_ast(root->right);
        free(root);
    }
}

int evaluate_ast(t_ast_node *root)
{
    if (!root)
        return 0;
    if (root->type == T_NUMBER)
        return root->value;
    int left_val = evaluate_ast(root->left);
    int right_val = evaluate_ast(root->right);
    switch (root->type)
    {
    case T_PLUS:
        return left_val + right_val;
    case T_MINUS:
        return left_val - right_val;
    case T_MULTIPLY:
        return left_val * right_val;
    default:
        return 0;
    }
}

int main()
{
    char *input;
    t_token *head = NULL;

    input = readline("calc> ");
    head = lexer(input);
    // print_tokens(head); // Uncomment this line if you want to see the tokens

    t_ast_node *root = parse_expression(&head);
    printf("Result: %d\n", evaluate_ast(root));

    free_ast(root);
    free(input);
    return 0;
}


