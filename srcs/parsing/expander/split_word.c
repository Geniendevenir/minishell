/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:54:19 by allan             #+#    #+#             */
/*   Updated: 2024/07/10 10:16:06 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_management(t_ast **root, t_ast **current, t_token	*token_list)
{
	if (!token_list->value || !*token_list->value)
	{
		delete_word(root, current);
		return (-1); //check valeur non utilise par exit status
	}
	else if (token_list->next)
	{
		if (handle_wildcard(current, &token_list) == 1)
			return (1);
	}
	else if (ft_strcmp((*current)->value, token_list->value) != 0)
		return (modify_word(current, token_list));
	return (0);
}

/* printf("AFTER EXPANDER:\n");
	token_print(&token_list); */
int split_word(t_ast **root, t_ast **current, t_env *env)
{
	size_t		i;
	t_token		*token_list;
	int			error;

	error = 0;
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return (1);
	token_init(&token_list);
	i = 0;
	while (i < ft_strlen((*current)->value))
	{
		error = split_one((*current)->value, &i, &token_list);
		if (error != 0)
		{
			token_free(&token_list);
			error_lexer(error);
			return (1);
		}
	}
	if (expander(&token_list, env, error) == 1) //free token list automatiquement
		return (1);
	error = word_management(root, current, token_list);
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
