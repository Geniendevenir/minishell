/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:54:19 by allan             #+#    #+#             */
/*   Updated: 2024/07/09 12:18:06 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	replace_word(t_ast **root, t_ast *node, t_ast *new_node)
{
	if (node->parent == NULL)  // node is the root
		*root = new_node;
	else if (node->parent->left == node)
		node->parent->left = new_node;
	if (new_node != NULL)
		new_node->parent = node->parent;
}

// Function to delete a node from the tree
void delete_word(t_ast **root, t_ast *node)
{
	if (node == NULL) 
		return ;
	if (node->left == NULL)
		replace_word(root, node, NULL); // Case 2: Node is a leaf
	else
		replace_word(root, node, node->left); // Case 3: Node has only left child
	free(node->value);
	free(node);
}

bool	modify_word(t_ast **node, t_token *token_list)
{
	char *temp;

	temp = (*node)->value;
	(*node)->value = ft_strdup(token_list->value);
	free(temp);
	if (!(*node)->value)
		return (1);
	return (0);
}


int split_word(t_ast **root, t_ast *current, t_env *env)
{
	size_t	i;
	t_token	*token_list;
	int		error;

	error = 0;
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return (1);
	token_init(&token_list);
	i = 0;
	while (i < ft_strlen(current->value))
	{
		error = split_one(current->value, &i, &token_list);
		if (error != 0)
		{
			token_free(&token_list);
			error_lexer(error);
			return (1);
		}
	}
	if (expander(&token_list, env, error) == 1) //free token list automatiquement
		return (1);
	printf("AFTER EXPANDER:\n");
	token_print(&token_list);
	if (!token_list->value || !*token_list->value) //wich one ?
		delete_word(root, current);
	else
	{
		if (ft_strcmp(current->value, token_list->value) != 0)
			error = modify_word(&current, token_list);
	}
	token_free(&token_list);
	return (error);
}
		
int	split_one(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;

	error = 0;
	if (cmd_line[*i] == '$')
	{
		if (cmd_line[*i + 1] == '?') //A CHECK
		{
			error = token_addback(token_list, "?", 2);
			env_special_token(token_list, 1);
			(*i) += 2;
		}
		else if (is_env(cmd_line[*i + 1]) == 1) //A CHECK
		{
			error = token_addback(token_list, "$", 2);
			env_special_token(token_list, 2);
			(*i)++;
		}
		else
			error = env_token(cmd_line, i, token_list);
	}
	else
		error = split_two(cmd_line, i, token_list);
	return (error);
}

int	split_two(const char *cmd_line, size_t *i, t_token **token_list)
{
	int	error;

	error = 0;
	if (cmd_line[*i] == '*' || is_wildcard(cmd_line, *i) == 0)
		error = wildcard_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '\"')
	{
		if (cmd_line[*i + 1] == '\"')
			*i += 2;
		else
			error = dquotes_token(cmd_line, i, token_list);
	}
	else if (cmd_line[*i] == '\'')
			error = squote_token(cmd_line, i, token_list);
	else
		error = word_token(cmd_line, i, token_list);
	return (error);
}
