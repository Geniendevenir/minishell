/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:27:35 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/15 16:42:31 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void init_t_word(t_word *word)
{
	word->redi_in = 0;
	word->redi_out = 0;
	word->here_doc = 0;
	word->append = 0;
	word->operator = 0;
	word->cmd = 0;
}

int enumToString(int enumValue) {
    if (enumValue >= 0 && enumValue < 27) 
	{
        return enumValue;
    } else {
        return -1;
    }
}

int get_list_length(t_token *head)
{
	int len;
	t_token *current = head;

	len = 0;
	while (current != NULL)
	{
		len++;
		current = current->next;
	}

	return (len);
}

void define_word(t_token **token_list, t_word *boolean, t_env *env)
{
	int i;
	int len_list;
	t_token *current;

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
		else if (current->type == TOKEN_APPENDOUT || current->type == TOKEN_REDIRECTIN ||
				current->type == TOKEN_REDIRECTOUT || current->type == TOKEN_HEREDOC ||
				current->type == TOKEN_PIPE || current->type == TOKEN_AND || current->type == TOKEN_OR)
			boolean->cmd = 0;
		else if (current->type == TOKEN_WORD)
			current->type = check_word(current->value, boolean, env);
		current = current->next;
	}
}


/* int main(int argc, char **argv, char **env)
{
    // Créer une liste de tokens en utilisant un tableau de chaînes de caractères
	t_all	*p;

	(void)argc;
	(void) **argv;
	p = init_all(env);
	if (!p || p == NULL)
		return (EXIT_FAILURE);
    int tokens[] = {
		TOKEN_REDIRECTIN,
		TOKEN_WORD,
		TOKEN_WORD,
		TOKEN_HEREDOC,
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_WORD,
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_WORD,
		TOKEN_WORD,
		TOKEN_PIPE,
		TOKEN_WORD,
		TOKEN_WORD,
		TOKEN_REDIRECTOUT,
		TOKEN_WORD
	};
	char *value[] = {"<","input.txt","/bin/cat","<<","LIMITER","'|'", "grep", "coucou","'|'","echo", "lol","'|'","ls","-l", ">", "output.txt"};
	t_word word;
	init_t_word(&word);
	define_word(value, tokens, &word, p->env);
	 int i = 0;
	while(tokens[i] && i < 16)
	{
        printf("%s            | token = %d\n",value[i], tokens[i]);
		i++;
	}
	free_all(p);
    return 0;
} */