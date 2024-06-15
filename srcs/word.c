/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:27:35 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/15 19:23:11 by Matprod          ###   ########.fr       */
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

void	define_word(t_token **token_list, t_word *boolean, t_env *env)
{
	int		i;
	int		len_list;
	t_token	*current;

	i = -1;
	len_list = get_list_length(*token_list);
	current = *token_list;
	while (current && ++i <= len_list)
	{
		if (current->type == TOKEN_REDIRECTIN)
			boolean->redi_in = 1;
		else if (current->type == TOKEN_REDIRECTOUT)
			boolean->redi_out = 1;
		else if (current->type == TOKEN_HEREDOC)
			boolean->here_doc = 1;
		else if (current->type == TOKEN_APPENDOUT)
			boolean->append = 1;
		else if (current->type == 10 || current->type == 7
			|| current->type == 8 || current->type == 9
			|| current->type == 6 || current->type == 4 || current->type == 5)
			boolean->cmd = 0;
		else if (current->type == TOKEN_WORD)
			current->type = check_word(current->value, boolean, env);
		current = current->next;
	}
}
