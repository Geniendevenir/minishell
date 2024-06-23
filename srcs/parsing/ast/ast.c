/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:30:25 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/23 13:04:19 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    NOT_DEFINE,
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
    TOKEN_ENV,
    TOKEN_WILDCARD,
    TOKEN_COUNT,
    WORD_FILEIN,
    WORD_FILEOUT,
    WORD_FILEOUT_APPEND,
    WORD_BUILTIN,
    WORD_ABSPATH,
    WORD_CMD,
    WORD_OPTION,
    WORD_LIMITER,
    WORD_STRING,
    WORD_ERROR,
    WORD_WTF
} s_type;

typedef struct
{
    s_type type;
    char* value;
} Token;

// Définition de la structure d'un nœud de l'AST
typedef struct s_ast
{
	s_type type;
	char *value;
	struct s_ast *left;
	struct s_ast *right;
	struct s_ast *parent;
} 	t_ast;

// Fonction pour créer un nouveau nœud de l'AST
t_ast	*createNode(s_type type, char* value)
{
	t_ast* node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = strdup(value); // a remplacer par ft_strdup
	if (!node->value)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

t_ast	*parseSubexpression(Token* tokens, int* index, int numTokens);

t_ast	*handleOpenParenthesis(Token* tokens, int* index, int numTokens, t_ast* current)
{
	(*index)++;
	t_ast* subTree = parseSubexpression(tokens, index, numTokens);
	if (current)
	{
		current->right = subTree;
		subTree->parent = current;
		current = subTree;
	}
	else
		current = subTree;
	return current;
}

t_ast	*handleCloseParenthesis(Token* tokens, int* index, t_ast* root)
{
	(*index)++;
	return root;
}

t_ast	*handleAndOr(Token* tokens, int* index, t_ast* root)
{
	t_ast	*newNode;

	newNode = createNode(tokens[*index].type, tokens[*index].value);
    newNode->left = root;
    if (root) root->parent = newNode;
    root = newNode;
    (*index)++;
    return root;
}

t_ast	*handleBuiltinCmdQuotes(Token* tokens, int* index, t_ast* current)
{
	t_ast* newNode;
	
	newNode = createNode(tokens[*index].type, tokens[*index].value);
	if (current)
	{
		current->right = newNode;
		newNode->parent = current;
		current = newNode;
	}
	else
		current = newNode;
	(*index)++;
	return current;
}

t_ast	*handleOption(Token* tokens, int* index, t_ast* current)
{
	t_ast*	newNode;

	newNode = createNode(tokens[*index].type, tokens[*index].value);
	if (current && current->type == WORD_BUILTIN)
	{
		if (current->right)
		{
			t_ast* temp = current->right;
			while (temp->right) temp = temp->right;
			temp->right = newNode;
			newNode->parent = temp;
		}
		else
		{
			current->right = newNode;
			newNode->parent = current;
		}
	}
	else
	{
		if (current)
		{
			current->right = newNode;
			newNode->parent = current;
			current = newNode;
		}
		else
			current = newNode;
	}
	(*index)++;
	return current;
}

t_ast* parseExpression(Token* tokens, int* index, int numTokens) 
{
	t_ast* root;
	t_ast* current;
	
    root = NULL;
    current = NULL;

    while (*index < numTokens)
	{
        s_type type = tokens[*index].type;
        
        if (type == TOKEN_OPENPAR)
		{
            current = handleOpenParenthesis(tokens, index, numTokens, current);
            if (!root)
				root = current;
        }
		else if (type == TOKEN_CLOSEPAR)
            return handleCloseParenthesis(tokens, index, root);
        else if (type == TOKEN_AND || type == TOKEN_OR)
		{
            root = handleAndOr(tokens, index, root);
            current = root;
        }
		else if (type == WORD_BUILTIN || type == WORD_CMD || type == TOKEN_DQUOTES || type == TOKEN_SQUOTES)
		{
            current = handleBuiltinCmdQuotes(tokens, index, current);
            if (!root)
				root = current;
        }
		else if (type == WORD_OPTION)
		{
            current = handleOption(tokens, index, current);
            if (!root)
				root = current;
        }
		else if (type == TOKEN_WHITESPACE)
            (*index)++;
        else
            return NULL;
    }
    return root;
}


t_ast* parseSubexpression(Token* tokens, int* index, int numTokens) {
    return parseExpression(tokens, index, numTokens);
}

// Fonction pour obtenir le nom d'un type de token
const char* getTokenTypeName(s_type type) {
    switch (type) {
        case NOT_DEFINE: return "NOT_DEFINE";
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
        case TOKEN_ENV: return "ENV";
        case TOKEN_WILDCARD: return "WILDCARD";
        case TOKEN_COUNT: return "COUNT";
        case WORD_FILEIN: return "FILEIN";
        case WORD_FILEOUT: return "FILEOUT";
        case WORD_FILEOUT_APPEND: return "FILEOUT_APPEND";
        case WORD_BUILTIN: return "BUILTIN";
        case WORD_ABSPATH: return "ABSPATH";
        case WORD_CMD: return "CMD";
        case WORD_OPTION: return "OPTION";
        case WORD_LIMITER: return "LIMITER";
        case WORD_STRING: return "STRING";
        case WORD_ERROR: return "ERROR";
        case WORD_WTF: return "WTF";
        default: return "UNKNOWN";
    }
}

// Fonction pour imprimer l'AST verticalement
void printAST(t_ast* node, int level) {
    if (node == NULL) return;
    
    // Print the right subtree first (with increased level)
    if (node->right) {
        printAST(node->right, level + 1);
    }

    // Print current node with proper indentation
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s: %s\n", getTokenTypeName(node->type), node->value);

    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }
}

int main()
{
    // Liste des tokens représentant une commande shell complexe
    Token tokens[] = {
        {TOKEN_OPENPAR, "("}, {WORD_BUILTIN, "echo"}, {WORD_OPTION, "salut"}, {TOKEN_AND, "&&"}, {WORD_BUILTIN, "echo"}, {WORD_OPTION, "yo"}, {TOKEN_CLOSEPAR, ")"}, 
        {TOKEN_AND, "&&"}, {TOKEN_OPENPAR, "("}, {WORD_BUILTIN, "echo"}, {WORD_OPTION, "coucou"}, {TOKEN_AND, "&&"}, 
        {TOKEN_OPENPAR, "("}, {WORD_BUILTIN, "echo"}, {WORD_OPTION, "prout"}, {TOKEN_AND, "&&"}, {TOKEN_OPENPAR, "("}, 
        {WORD_BUILTIN, "echo"}, {WORD_OPTION, "okayyyyyy"}, {TOKEN_CLOSEPAR, ")"}, {TOKEN_CLOSEPAR, ")"}, {TOKEN_CLOSEPAR, ")"}, 
        {TOKEN_AND, "&&"}, {TOKEN_OPENPAR, "("}, {WORD_BUILTIN, "echo"}, {WORD_OPTION, "flop"}, {TOKEN_AND, "&&"}, 
        {WORD_BUILTIN, "echo"}, {WORD_OPTION, "bye"}, {TOKEN_CLOSEPAR, ")"}
    };
    int numTokens = sizeof(tokens) / sizeof(tokens[0]);
    int index = 0;

    t_ast* ast = parseExpression(tokens, &index, numTokens);
    
    // Affichage de l'AST pour vérification
    printAST(ast, 0);

    return 0;
}

// ( echo salut && echo yo ) && ( echo coucou && ( echo prout && ( echo salut ) ) ) && ( echo flop && echo bye )

/* t_ast* TRUE_VERSIONparseExpression(t_token *token_list, int* index, int numTokens)
{
	t_ast *root;
	t_ast *current;
	t_token *tokens;

	root = NULL;
	current = NULL;
	tokens = token_list;
	while (*index < numTokens)
	{
		if (tokens->type == TOKEN_OPENPAR)
		{
            current = handleOpenParenthesis(tokens, index, numTokens, current);
            if (!root)
				root = current;
        }
		else if (tokens->type == TOKEN_CLOSEPAR)
            return handleCloseParenthesis(tokens, index, root);
        else if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR)
		{
            root = handleAndOr(tokens, index, root);
            current = root;
        }
		else if (tokens->type == WORD_BUILTIN || tokens->type == WORD_CMD || tokens->type == TOKEN_DQUOTES || tokens->type == TOKEN_SQUOTES)
		{
            current = handleBuiltinCmdQuotes(tokens, index, current);
            if (!root) root = current;
        }
		else if (tokens->type == WORD_OPTION)
		{
            current = handleOption(tokens, index, current);
            if (!root)
				root = current;
        }
		else if (tokens->type == TOKEN_WHITESPACE) 
            tokens = token->next;
        else
            return (NULL);
    }
    return (root);
} */