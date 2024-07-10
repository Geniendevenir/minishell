/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:49:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/10 16:20:01 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		"|WORD_SQLIMITER",
	};
    if (current->type >= 0 && current->type < 30) {
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
    //printf("%s: %s\n",getAST_Class(node), node->value);
	//printf("|%s | state = %d\n",node->value,node->state);
	//printf("|%s sub = %d\n",node->value,node->subshell);
    // Print the left subtree (with increased level)
    if (node->left) {
        printAST(node->left, level + 1);
    }
}