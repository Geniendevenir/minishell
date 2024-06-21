/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:30:25 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/21 18:30:18 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


enum s_type{
    NOT_DEFINE,
    TOKEN_CMD,
    TOKEN_DQUOTES,
    TOKEN_SQUOTES,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_PIPE,
    TOKEN_REDIRECTIN,
    TOKEN_REDIRECTOUT,
    TOKEN_HEREDOC,
    TOKEN_APPENDOUT,
    TOKEN_LIMITER,
    TOKEN_OPENPAR,
    TOKEN_CLOSEPAR,
    TOKEN_WHITESPACE,
};

// Définition de la structure d'un token
typedef struct {
    enum s_type type;
    char* value;
} Token;

// Définition de la structure d'un nœud de l'AST
typedef struct s_ast {
    enum s_type type;
    char *value;
    struct s_ast *left;
    struct s_ast *right;
    struct s_ast *parent;
} t_ast;

t_ast *createNode(enum s_type type, char *value)
{
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = type;
    node->value = strdup(value); //ft_strdup
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return (node);
}



// Fonction pour analyser une sous-expression et construire l'AST
t_ast* parseExpression(Token* tokens, int* index, int numTokens);

t_ast* parseSubexpression(Token* tokens, int* index, int numTokens) {
    return parseExpression(tokens, index, numTokens);
}

t_ast* parseExpression(Token* tokens, int* index, int numTokens) {
    t_ast* root = NULL;
    t_ast* current = NULL;

    while (*index < numTokens) {
        enum s_type type = tokens[*index].type;
        
        if (type == TOKEN_OPENPAR) {
            (*index)++;
            t_ast* subTree = parseSubexpression(tokens, index, numTokens);
            if (current) {
                current->right = subTree;
                subTree->parent = current;
                current = subTree;
            } else {
                root = subTree;
                current = root;
            }
        } else if (type == TOKEN_CLOSEPAR) {
            (*index)++;
            return root;
        } else if (type == TOKEN_AND || type == TOKEN_OR) {
            t_ast* newNode = createNode(type, tokens[*index].value);
            newNode->left = root;
            if (root) root->parent = newNode;
            root = newNode;
            current = root;
            (*index)++;
        } else if (type == TOKEN_CMD || type == TOKEN_DQUOTES || type == TOKEN_SQUOTES) {
            t_ast* newNode = createNode(type, tokens[*index].value);
            if (current) {
                current->right = newNode;
                newNode->parent = current;
                current = newNode;
            } else {
                root = newNode;
                current = root;
            }
            (*index)++;
        } else if (type == TOKEN_WHITESPACE) {
            (*index)++; // Ignorer les espaces
        } else {
            return NULL; // Type de token non attendu
        }
    }
    return root;
}


const char* getTokenTypeName(enum s_type type) {
    switch (type) {
        case NOT_DEFINE: return "NOT_DEFINE";
        case TOKEN_CMD: return "CMD";
        case TOKEN_DQUOTES: return "DQUOTES";
        case TOKEN_SQUOTES: return "SQUOTES";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_REDIRECTIN: return "REDIRECTIN";
        case TOKEN_REDIRECTOUT: return "REDIRECTOUT";
        case TOKEN_HEREDOC: return "HEREDOC";
        case TOKEN_APPENDOUT: return "APPENDOUT";
        case TOKEN_LIMITER: return "LIMITER";
        case TOKEN_OPENPAR: return "OPENPAR";
        case TOKEN_CLOSEPAR: return "CLOSEPAR";
        case TOKEN_WHITESPACE: return "WHITESPACE";
        default: return "UNKNOWN";
    }
}

// Fonction pour imprimer l'AST sous forme d'arbre
void printAST(t_ast* node, int level) {
    if (node == NULL) return;
    
    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }

    // Print current node with proper indentation
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s: %s\n", getTokenTypeName(node->type), node->value);

    // Print the right subtree first (with increased level)
    if (node->right) {
        printAST(node->right, level + 1);
    }
}

int main() {
    // Exemple de tokens représentant une commande
    Token tokens[] = {
        {TOKEN_OPENPAR, "("},
        {TOKEN_CMD, "echo salut"},
        {TOKEN_AND, "&&"},
        {TOKEN_CMD, "echo yo"},
        {TOKEN_CLOSEPAR, ")"},
        {TOKEN_AND, "&&"},
        {TOKEN_OPENPAR, "("},
        {TOKEN_CMD, "echo coucou"},
        {TOKEN_AND, "&&"},
        {TOKEN_OPENPAR, "("},
        {TOKEN_CMD, "echo prout"},
        {TOKEN_AND, "&&"},
        {TOKEN_OPENPAR, "("},
        {TOKEN_CMD, "echo salut"},
        {TOKEN_CLOSEPAR, ")"},
        {TOKEN_CLOSEPAR, ")"},
        {TOKEN_CLOSEPAR, ")"},
        {TOKEN_AND, "&&"},
        {TOKEN_OPENPAR, "("},
        {TOKEN_CMD, "echo flop"},
        {TOKEN_AND, "&&"},
        {TOKEN_CMD, "echo bye"},
        {TOKEN_CLOSEPAR, ")"}
    };
    int numTokens = sizeof(tokens) / sizeof(tokens[0]);
    int index = 0;

    t_ast* ast = parseExpression(tokens, &index, numTokens);
    
    // Affichage de l'AST pour vérification
    printAST(ast, 0);

    return 0;
}