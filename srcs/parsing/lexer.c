/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:38:47 by allan             #+#    #+#             */
/*   Updated: 2024/06/03 22:01:31 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parser(char *cmd_line)
{
	t_token	*token_list;
	
	if (!cmd_line)
		return ;
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return ;
	token_list->state = STATE_START;
	token_list->type = NOT_DEFINE;
	token_list->value = NULL;
	token_list->len = 0;
	token_list->next = NULL;
	lexer(cmd_line, &token_list); //Tokenize -> Return Token_List
	//expander(); // Verifie chaque Token extend les $ENV/supprime les quotes
	//syntax_check(); // Utilime check structure de l'input
	//ast(); // Tour de Controle de l'execution 
}

void lexer(char *cmd_line, t_token **token_list)
{
	size_t	i;
	
	if (check_quotes(cmd_line) || check_semicolon(cmd_line))
	{
		write(2, "Error syntax\n", 13); // check la veritable erreur a renvoyer
		return ;
	}
	i = 0;
	while (i <= ft_strlen(cmd_line))
	{
		tokenizer(cmd_line, &i, token_list);
		printf("i each turn = %ld\n", i);
		printf("cmd_line = %s\n", cmd_line + i);
	}
	printf("cmd_line[end] = %d\n", cmd_line[i]);
	token_print(token_list);
}

void	tokenizer(const char *cmd_line, size_t *i, t_token **token_list)
{
	if (is_whitespace(cmd_line[*i]))
		whitespace_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '\"')
		dquote_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '\'')
		squote_token(cmd_line, i, token_list);
	else if (cmd_line[*i] == '(')
		inpar_token(i, token_list);
	else if (cmd_line[*i] == ')')
		outpar_token(i, token_list);
	else if (cmd_line[*i] == '|')
	{
		if (cmd_line[*i + 1] == '|')
			or_token(i, token_list);
		else
			pipe_token(i, token_list);
	}
	else if (cmd_line[*i] == '&' && cmd_line[*i + 1] == '&')
			and_token(i, token_list);
	else
		tokenizer_partwo(cmd_line, i, token_list);
	printf("ft_tokenizer = %ld\n", (*i));
}

void	tokenizer_partwo(const char *cmd_line, size_t *i, t_token **token_list)
{
	if (cmd_line[*i] == '<')
	{
		if (cmd_line[*i + 1] == '<')
			heredoc_token(i, token_list);
		else
			inputre_token(i, token_list);
	}
	else if (cmd_line[*i] == '>')
	{
		if (cmd_line[*i + 1] == '>')
			outputapp_token(i, token_list);
		else
			outputre_token(i, token_list);
	}
	else if (cmd_line[*i] == '$' && is_env(cmd_line[*i + 1]))
		env_token(cmd_line, i, token_list);
	else
		lexical_token(cmd_line, i, token_list);
}