/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:58:09 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 16:00:37 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envv(t_env **env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (current->key)
			printf("%s\n", current->key);
		current = current->next;
	}
}

int	handle_wildcard(t_ast **cur, t_token **token_list)
{
	t_token	*token;
	int		error;

	token = *token_list;
	error = 0;
	if ((*cur)->type == 19 || (*cur)->type == 18 || (*cur)->type == 20)
		error_expander(*cur, 1);
	else if ((*cur)->type == WORD_CMD || (*cur)->type == WORD_OPTION)
	{
		if (ft_strcmp((*cur)->value, token->value) != 0)
		{
			error = modify_word(cur, *token_list);
			if (error != 0)
				return (error);
		}
		token = token->next;
		while (token)
		{
			if (insert_word(cur, token) == 1)
				return (1);
			*cur = (*cur)->left;
			token = token->next;
		}
	}
	return (0);
}

void	env_dquotes_foward(const char *cmd_line, size_t *j, int *option)
{
	if (cmd_line[*j] == '?')
	{
		*j += 1;
		*option = 2;
	}
	else if ((cmd_line[*j] >= '0' && cmd_line[*j] <= '9')
		|| cmd_line[*j] == '*')
		*j += 1;
	else
	{
		while (is_valid_env(cmd_line[*j]) == 0)
			*j += 1;
	}
}
