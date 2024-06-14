/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/06/08 11:04:51 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parser(char *cmd_line, t_env *env)
{
	t_token	*token_list;
	
	if (!cmd_line)
		return ;
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return ;
	token_init(token_list);
	if (lexer(cmd_line, &token_list) == 1)
		return ;
	printf("AFTER LEXER:\n");
	token_print(&token_list);
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