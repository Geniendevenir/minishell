/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:27:35 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/16 15:59:34 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_t_word(t_word *word)
{
	word->redi_in = 0;
	word->redi_out = 0;
	word->here_doc = 0;
	word->append = 0;
	word->operator = 0;
	word->cmd = 0;
}

int	get_list_length(t_token *head)
{
	int		len;
	t_token	*current;

	current = head;
	len = 0;
	while (current != NULL)
	{
		len++;
		current = current->next;
	}
	return (len);
}
bool	if_define_word(t_token *current)
{
	if (current->type == 10 || current->type == 7 
		|| current->type == 8 || current->type == 9
		|| current->type == 6 || current->type == 4 || current->type == 5)
		return (1);
	else
		return (0);
}

void	print_and_free_define_word(t_token **list, t_token *current)
{
	print_error_cmd_not_found(current);
	token_free(list);
}

bool	define_word(t_token **token_list, t_word *boolean, t_env *env)
{
	t_token	*current;

	current = *token_list;
	while (current)
	{
		if (current->type == TOKEN_REDIRECTIN)
			boolean->redi_in = 1;
		else if (current->type == TOKEN_REDIRECTOUT)
			boolean->redi_out = 1;
		else if (current->type == TOKEN_HEREDOC)
			boolean->here_doc = 1;
		else if (current->type == TOKEN_APPENDOUT)
			boolean->append = 1;
		else if (if_define_word(current) == 1)
			boolean->cmd = 0;
		else if (current->type == TOKEN_WORD)
			current->type = check_word(current->value, boolean, env);
		if (current->type == WORD_ERROR)
		{
			print_and_free_define_word(token_list, current);
			return (1);
		}
		current = current->next;
	}
	return (0);
}
