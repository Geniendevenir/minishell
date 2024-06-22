/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   true_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/22 20:10:47 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Définition de la structure d'un nœud de l'AST


// Fonction pour créer un nouveau nœud de l'AST
t_ast* parseSubexpression(t_token **tokens);

const char* getAST_Class(t_ast *current)
{
	const char* Token_Class[] = {
    	"NOT_DEFINE",
		"TOKEN_WORD",
		"TOKEN_DQUOTES",
		"TOKEN_SQUOTES",
		"TOKEN_AND",
		"TOKEN_OR",
		"TOKEN_PIPE",
		"TOKEN_REDIRECTIN",
		"TOKEN_REDIRECTOUT",
		"TOKEN_HEREDOC",
		"TOKEN_APPENDOUT",
		"TOKEN_LIMITER",
		"TOKEN_OPENPAR",
		"TOKEN_CLOSEPAR",
		"TOKEN_WHITESPACE",
		"TOKEN_ENV",
		"TOKEN_WILDCARD",
		"TOKEN_COUNT",
		"WORD_FILEIN",
		"WORD_FILEOUT",
		"WORD_FILEOUT_APPEND",
		"WORD_BUILTIN",
		"WORD_ABSPATH",
		"WORD_CMD",
		"WORD_OPTION",
		"WORD_LIMITER",
		"WORD_STRING",
		"WORD_ERROR",
		"WORD_WTF",
	};
    if (current->type >= 0 && current->type < 28) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
	
}

t_ast	*createNode(enum s_type type, char* value)
{
	t_ast* node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value); // a remplacer par ft_strdup
	if (!node->value)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

void addRightChild(t_ast* current, t_ast* newNode) {
    if (current == NULL || newNode == NULL) {
        return;
    }
    
    current->right = newNode;
    newNode->parent = current;
    current = newNode; // Cela n'est plus nécessaire ici, car current devrait déjà pointer vers newNode
}

t_ast	*handleOpenParenthesis(t_token **tokens, t_ast* current)
{
	//ajouter toke next ?
	*tokens = (*tokens)->next;
	t_ast* subTree = parseSubexpression(tokens);
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

t_ast	*handleCloseParenthesis(t_token **tokens, t_ast* root)
{
	//rajouter token next ?
	*tokens = (*tokens)->next;
	return (root);
}

t_ast	*handleAndOr(t_token **tokens, t_ast* root)
{
	t_ast	*newNode;

	newNode = createNode((*tokens)->type, (*tokens)->value);
    newNode->left = root;
    if (root) root->parent = newNode;
    root = newNode;
	*tokens = (*tokens)->next;
    return root;
}

t_ast	*handleBuiltinCmdQuotes(t_token **tokens, t_ast* current)
{
	t_ast* newNode;
	
	newNode = createNode((*tokens)->type, (*tokens)->value);
	if (current)
	{
		current->right = newNode;
		newNode->parent = current;
		current = newNode;
	}
	else
		current = newNode;
	*tokens = (*tokens)->next;
	return current;
}

t_ast	*handleOption(t_token **tokens, t_ast* current)
{
	t_ast*	newNode;

	newNode = createNode((*tokens)->type, (*tokens)->value);
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
	*tokens = (*tokens)->next;
	//ajouter token next ?
	return current;
}


//definir root pour factoriser
t_ast* parseExpression(t_token **token_list)
{
	t_ast *root;
	t_ast *current;
	t_token *tokens;

	root = NULL;
	current = NULL;
	tokens = *token_list;
	while (tokens != NULL) {
		// Traiter le jeton courant
		if (tokens->type == TOKEN_OPENPAR)
		{
			current = handleOpenParenthesis(&tokens, current);
			if (!root)
				root = current;
		}
		else if (tokens->type == TOKEN_CLOSEPAR)
			return handleCloseParenthesis(&tokens, root);
		else if (tokens->type == TOKEN_AND || tokens->type == TOKEN_OR)
		{
			root = handleAndOr(&tokens, root);
			current = root;
		}
		else if (tokens->type == WORD_BUILTIN || tokens->type == WORD_CMD || tokens->type == TOKEN_DQUOTES || tokens->type == TOKEN_SQUOTES)
		{
			current = handleBuiltinCmdQuotes(&tokens, current);
			if (!root) {
				root = current;
			}
		}
		else if (tokens->type == WORD_OPTION)
		{
			current = handleOption(&tokens, current);
			if (!root) {
				root = current;
			}
		}
		else if (tokens->type == TOKEN_WHITESPACE)
            tokens = tokens->next;
        else
            return NULL;
	}

	return (root);

}
t_ast	*parseSubexpression(t_token **tokens)
{
	return parseExpression(tokens);
}

// Fonction pour obtenir le nom d'un type de token

/* int main()
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
} */

// ( echo salut && echo yo ) && ( echo coucou && ( echo prout && ( echo okkkkkkkk ) ) ) && ( echo flop && echo bye )

/* t_ast* parseExpression(t_token *token_list, int* index, int numTokens)
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