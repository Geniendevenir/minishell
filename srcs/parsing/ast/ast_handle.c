/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:48:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/01 16:37:58 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*handle_open_parenthesis(t_token **tokens, t_ast	*current)
{
	t_ast	*sub_tree;
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	sub_tree = parse_subexpression(tokens);
	if (current)
	{
		current->right = sub_tree;
		sub_tree->parent = current;
		current = sub_tree;
	}
	else
		current = sub_tree;
	return (current);
}

t_ast	*handle_close_parenthesis(t_token **tokens, t_ast	*root)
{
	t_token	*temp;

	free_token_and_next_in_ast(tokens, &temp);
	return (root);
}

void	handle_redirect_or_pipe(t_token **tokens, t_ast **current, t_ast **root, t_ast *save_operator)
{
	t_ast	*new_node;
	t_token	*temp;
	enum s_type	enum_temp;
	char	*value_temp;
	//int is_operator;

	value_temp = NULL;
	//is_operator = 1;
	printf("address of save_operator in handle redirect= %p\n", save_operator);
	while (*current && (*current)->left)
	{
			*current = (*current)->left;
	}
	new_node = create_node((*tokens)->type, (*tokens)->value); //proteger
	if(*current)
	{
		printf("current  in if current redirect= %s \n",(*current)->value);
		swap_child_left_with_else((*current), new_node);
	}
	if (!*root)//si pas de root
	{
		printf(" in root null\n");
		*current = new_node;
		*root = *current;
	}
	else
	{
		//*current = (*current)->left;
		printf("current = %s | current left = %s\n",(*current)->value, (*current)->left->value);
		*current = (*current)->left;
		while ((save_operator != (*current)) &&((*current)->parent && *current))
		{
			printf("in while handle redirect current = %s | current parent = %s | address of parent = %p\n", (*current)->value, (*current)->parent->value, (*current)->parent);
			if (value_temp != NULL)
				free(value_temp);
			if ((*current)->parent == save_operator)
				break ;
			value_temp = ft_strdup((*current)->parent->value); //proteger
			enum_temp = (*current)->parent->type;
			free((*current)->parent->value);
			(*current)->parent->value = ft_strdup((*current)->value); // protger
			(*current)->parent->type = (*current)->type;
			free((*current)->value);
			(*current)->value = ft_strdup(value_temp); //proteger
			(*current)->type = enum_temp;
			*current = (*current)->parent;
		}
		while (*current && (*current)->left)
			(*current) = (*current)->left;
	}
	free_token_and_next_in_ast(tokens, &temp);
}

t_ast	*handle_priorities(t_token **tokens, t_ast	*root)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return (NULL);
	new_node->left = root;
	if (root)
		root->parent = new_node;
	root = new_node;
	free_token_and_next_in_ast(tokens, &temp);
	return (root);
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
