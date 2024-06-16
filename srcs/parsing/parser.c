/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/06/16 13:19:31 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
		TO DO:
	1 - Concatener env_token: OK
	2 - Verifier tous les cas d'erreurs du lexer: OK
	3 - Finir expander env: OK
	4 - RAJOUTER LE CAS $? dans expander env
	4 - Verifier leak et erreurs expander env
	5 - Faire le relink
	6 - Wildcard
	7 - Syntax 
*/

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
	token_print_amazing(&token_list);
	if (expander(&token_list, env) == 1)
		return ;
	//syntax_check(); // Ultime check structure de l'input
	//ast(); // Tour de Controle de l'execution
	printf("\nAFTER EXPANDER:\n");
	token_print(&token_list);
	//token_print_amazing(&token_list);
	token_free(&token_list);
}