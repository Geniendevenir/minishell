/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/06/16 16:50:00 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parser(char *cmd_line, t_env *env) // a rajouter env quand expander fini
{
	t_token	*token_list;
	t_word word;

	if (!cmd_line)
		return ;
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return ;
	token_init(token_list);
	if (lexer(cmd_line, &token_list) == 1)
		return ;
	//printf("AFTER LEXER:\n");
	//token_print_amazing(&token_list);
	if (check_syntax(&token_list) == 1)
		return ;
	init_t_word(&word);
	if (define_word(&token_list, &word, env))
		return ;
	//printf("\n\n\nAFTER DEFINE WORD:\n");
	//token_print_amazing(&token_list);
	//if (expander(&token_list, env) == 1)
		//return ;
	//syntax_check(); // Ultime check structure de l'input
	//ast(); // Tour de Controle de l'execution
	//printf("AFTER EXPANDER:\n");
	//token_print(&token_list);
	//token_print_amazing(&token_list);
	token_free(&token_list);
}

