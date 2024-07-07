/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_builtin_and_cmd.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 19:57:09 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/06 21:15:00 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	part_builtin_and_cmd(t_ast_ptr **list, t_ast **new_node,
	t_ast **temp_current)
{
	if (is_redirect_enum((*list)->current->type))
	{
		*temp_current = (*list)->current;
		while ((*list)->current->left)
			(*list)->current = (*list)->current->left;
		(*list)->current->left = *new_node;
		(*new_node)->parent = (*list)->current;
		(*list)->current = *temp_current;
	}
	else if ((*list)->current->left)
	{
		(*list)->current->right = (*new_node);
		(*new_node)->parent = (*list)->current;
		(*list)->current = (*new_node);
	}
}

t_ast	*handle_builtin_and_cmd(t_token **tokens, t_ast_ptr	**list,
	int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;
	t_ast	*temp_current;

	new_node = create_node(*tokens, sub_shell);
	(*list)->last_cmd = new_node;
	if ((*list)->current)
		part_builtin_and_cmd(list, &new_node, &temp_current);
	else
		(*list)->current = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return ((*list)->current);
}

t_ast	*handle_option(t_token **tokens, t_ast_ptr **list, int sub_shell)
{
	t_ast	*new_node;
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
