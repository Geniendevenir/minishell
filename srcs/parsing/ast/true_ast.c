/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   true_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 18:41:34 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 17:05:07 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool if_priorities(t_token **tokens)
{
	if((*tokens)->type == 19 || (*tokens)->type == 20 
	|| (*tokens)->type == 25 || (*tokens)->type == 18 || (*tokens)->type == 6 
	|| (*tokens)->type == 5 || (*tokens)->type == 4)
		return (1);
	else
		return (0);
}

t_ast	*get_first_parent(t_ast *current)
{
	while (current && current->parent)
		current = current->parent;
	return (current);
}



t_ast* parseExpression(t_token **tokens)
{
	t_ast *root;
	t_ast *current;

	root = NULL;
	current = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_OPENPAR)
		{
			current = handleOpenParenthesis(tokens, current);
			if (!root)
				root = current;
		}
		else if ((*tokens)->type == TOKEN_CLOSEPAR)
			return handleCloseParenthesis(tokens, root);
		else if (if_priorities(tokens))
		{
			root = handle_priorities(tokens, root);
			current = root;
		}
		else if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD || (*tokens)->type == WORD_ABSPATH)
		{
			current = handleBuiltinCmdQuotes(tokens, current);
			if (!root)
				root = current;
		}
		else if ((*tokens)->type == WORD_OPTION)
			current = handleOption(tokens, current);
		else
            (*tokens) = (*tokens)->next;
	}
	current = get_first_parent(current);
	return (current);
}
t_ast	*parseSubexpression(t_token **tokens)
{
	return parseExpression(tokens);
}