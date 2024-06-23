/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/06/23 13:09:00 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*

TO DO:
-Finir / Corriger AST
-Builtins
-Check Leak
-Error Management de TOUT le code
-Premier Tour de norminette
-Execution
-Cas $?
-Test Google Doc

*/

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
	if (expander(&token_list, env) == 1)
		return ;
	/* if (check_syntax(&token_list) == 1)
		return ; */
	printf("\n\n\nAFTER EXPANDER AND SYNTAX:\n");
	token_print_amazing(&token_list);
	init_t_word(&word);
	if (define_word(&token_list, &word, env))
		return ;
	printf("\n\n\nAFTER define word:\n");
	token_print_amazing(&token_list);
	t_ast *ast = parseExpression(&token_list);
	printAST(ast,0);
	//syntax_check(); // Ultime check structure de l'input
	//ast(); // Tour de Controle de l'execution
	//printf("AFTER EXPANDER:\n");
	//token_print(&token_list);
	token_free(&token_list);
}

