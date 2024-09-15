/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/09/15 13:08:02 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_token_list(t_token **token, char *cmd)
{
	if (cmd[0] == '\0')
		return (1);
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (1);
	token_init(token);
	return (0);
}

int	parser(char *cmd_line, t_ast **ast, t_all **p)
{
	t_token	*token_list;
	t_word	word;
	int		error;
	int		sub_shell;

	error = 0;
	if (create_token_list(&token_list, cmd_line) == 1)
		return (1);
	if (lexer(cmd_line, &token_list, error) == 1)
		return (1);
	if (check_syntax(token_list) == 1)
	{
		token_free(&token_list);
		return (1);
	}
	here_doc(&token_list, p);
	if (here_doc_check_file(*p, token_list) == 1)
		return (1);
	init_t_word(&word);
	if (define_word(&token_list, &word))
		return (1);
	sub_shell = 0;
	*ast = parse_expression(&token_list, sub_shell);
	token_free(&token_list);
	return (0);
}
