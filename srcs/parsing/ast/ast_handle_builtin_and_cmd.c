/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_builtin_and_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:57:09 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 19:37:49 by allan            ###   ########.fr       */
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

t_ast	*handle_builtin_and_cmd(t_token **tokens, t_ast	*current, int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node(*tokens, sub_shell);
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

t_ast	*handle_option(t_token **tokens, t_ast *current, int sub_shell)
{
	t_ast	*new_node;
	t_ast	*temp;
	t_token	*tmp;

	new_node = create_node(*tokens, sub_shell);
	if (current && (current->type == WORD_BUILTIN || current->type == WORD_CMD))
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

void	handle_builtin_cmd_or_option(t_token **tokens, t_ast_ptr **list,
	int sub_shell)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH)
	{
		(*list)->current = handle_builtin_and_cmd(tokens, (*list)->current,
				sub_shell);
		if (!(*list)->root)
			(*list)->root = (*list)->current;
	}
	else if ((*tokens)->type == WORD_OPTION)
		(*list)->current = handle_option(tokens, (*list)->current, sub_shell);
}
