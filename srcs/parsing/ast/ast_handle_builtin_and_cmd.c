/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_builtin_and_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:57:09 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/05 23:50:19 by Matprod          ###   ########.fr       */
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

void	part_handle_option(t_ast_ptr **list, t_ast **new_node, t_ast **temp)
{
	if ((*list)->current->left)
	{
		(*temp) = (*list)->current->left;
		while ((*temp)->left)
			(*temp) = (*temp)->left;
		(*temp)->left = (*new_node);
		(*new_node)->parent = *temp;
	}
	else
	{
		(*list)->current->left = (*new_node);
		(*new_node)->parent = (*list)->current;
	}
}

t_ast	*handle_builtin_and_cmd(t_token **tokens, t_ast_ptr	**list, int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node(*tokens, sub_shell);
	(*list)->last_cmd = new_node;
	if ((*list)->current)
	{
		(*list)->current->right = new_node;
		new_node->parent = (*list)->current;
		(*list)->current = new_node;
	}
	else
		(*list)->current = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return ((*list)->current);
}

t_ast	*handle_option(t_token **tokens, t_ast_ptr **list, int sub_shell)
{
	t_ast	*new_node;
	t_ast	*temp;
	t_token	*tmp;

	new_node = create_node(*tokens, sub_shell);
	while ((*list)->last_cmd->left)
		(*list)->last_cmd = (*list)->last_cmd->left;
	(*list)->last_cmd->left = new_node;
	new_node->parent = (*list)->last_cmd;
	(*list)->current = new_node;
	free_token_and_next_in_ast(tokens, &tmp);
	return ((*list)->current);
}

void	handle_builtin_cmd_or_option(t_token **tokens, t_ast_ptr **list,
	int sub_shell)
{
	if ((*tokens)->type == WORD_BUILTIN || (*tokens)->type == WORD_CMD
		|| (*tokens)->type == WORD_ABSPATH)
	{
		(*list)->current = handle_builtin_and_cmd(tokens, list,
				sub_shell);
		if (!(*list)->root)
			(*list)->root = (*list)->current;
	}
	else if ((*tokens)->type == WORD_OPTION)
		(*list)->current = handle_option(tokens, list, sub_shell);
}
