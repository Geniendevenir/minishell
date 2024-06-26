/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:50:17 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 16:04:47 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*createNode(enum s_type type, char* value)
{
	t_ast* node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = ft_strdup(value);
	if (!node->value)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return (node);
}

void	swap_child_left(t_ast* current, t_ast* newNode)
{
    if (current == NULL || newNode == NULL)
        return;
    
    current->left = newNode;
    newNode->parent = current;
    current = newNode;
}

void	swap_child_right(t_ast* current, t_ast* newNode)
{
    if (current == NULL || newNode == NULL)
        return;
    
    current->left = newNode;
    newNode->parent = current;
    current = newNode;
}

const char* getAST_Class(t_ast *current) //a enlever
{
	const char* Token_Class[] = {
    	"|NOT_DEFINE",
		"|TOKEN_WORD",
		"|TOKEN_DQUOTES",
		"|TOKEN_SQUOTES",
		"|TOKEN_AND",
		"|TOKEN_OR",
		"|TOKEN_PIPE",
		"|TOKEN_REDIRECTIN",
		"|TOKEN_REDIRECTOUT",
		"|TOKEN_HEREDOC",
		"|TOKEN_APPENDOUT",
		"|TOKEN_LIMITER",
		"|TOKEN_OPENPAR",
		"|TOKEN_CLOSEPAR",
		"|TOKEN_WHITESPACE",
		"|TOKEN_ENV",
		"|TOKEN_WILDCARD",
		"|TOKEN_COUNT",
		"|WORD_FILEIN",
		"|WORD_FILEOUT",
		"|WORD_FILEOUT_APPEND",
		"|WORD_BUILTIN",
		"|WORD_ABSPATH",
		"|WORD_CMD",
		"|WORD_OPTION",
		"|WORD_LIMITER",
		"|WORD_STRING",
		"|WORD_ERROR",
		"|WORD_WTF",
	};
    if (current->type >= 0 && current->type < 28) {
        return Token_Class[current->type];
    } else {
        return "NULL";
    }
}

void printAST(t_ast* node, int level)// a enlever
{
    if (node == NULL) return;
    
    // Print the right subtree first (with increased level)
    if (node->right) {
        printAST(node->right, level + 1);
    }

    // Print current node with proper indentation
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s: %s\n",getAST_Class(node), node->value);

    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }
}

