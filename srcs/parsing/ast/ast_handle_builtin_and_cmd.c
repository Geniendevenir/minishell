/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_builtin_and_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:57:09 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 20:03:19 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_cmd_or_option(t_token **tokens)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH || (*tokens)->type == WORD_OPTION)
		return (1);
	else
		return (0);
}

void	part_handle_option(t_ast **current, t_ast **new_node, t_ast **temp)
{
	if ((*current)->left)
	{
		(*temp) = (*current)->left;
		while ((*temp)->left)
			(*temp) = (*temp)->left;
		(*temp)->left = (*new_node);
		(*new_node)->parent = *temp;
	}
	else
	{
		(*current)->left = (*new_node);
		(*new_node)->parent = (*current);
	}
}

t_ast	*handle_builtin_and_cmd(t_token **tokens, t_ast	*current)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (current)
	{
		current->right = new_node;
		new_node->parent = current;
		current = new_node;
	}
	else
		current = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return (current);
}

t_ast	*handle_option(t_token **tokens, t_ast *current)
{
	t_ast	*new_node;
	t_ast	*temp;
	t_token	*tmp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (current && current->type == WORD_BUILTIN)
		part_handle_option(&current, &new_node, &temp);
	else
	{
		if (current)
			swap_child_left(current, new_node);
		else
			current = new_node;
	}
	free_token_and_next_in_ast(tokens, &tmp);
	return (current);
}

void	handle_builtin_cmd_or_option(t_token **tokens, t_ast **current, t_ast **root)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH)
	{
		*current = handle_builtin_and_cmd(tokens, *current);
		if (!*root)
			*root = *current;
	}
	else if ((*tokens)->type == WORD_OPTION)
		*current = handle_option(tokens, *current);
}

