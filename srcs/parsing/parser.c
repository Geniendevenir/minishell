/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/06/27 14:12:53 by Matprod          ###   ########.fr       */
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
t_token	*duplicate_token(const t_token *token)
{
	t_token *dup;
	
	if (token == NULL)
		return (NULL);
	dup = (t_token *)malloc(sizeof(t_token));
	if (dup == NULL)
		return NULL;
	dup->type = token->type;
	dup->state = token->state;
	dup->len = token->len;
	dup->value = ft_strdup(token->value);
	if (!dup->value)
		return (NULL);
	dup->next = NULL;
	return (dup);
}

// Fonction pour dupliquer une liste de structures t_token
t_token	*duplicate_token_list(const t_token *head)
{
	t_token			*new_head;
	const t_token	*current_src;
	t_token			*current;
	
	if (head == NULL)
		return NULL;
	new_head = duplicate_token(head);
	if (new_head == NULL)
		return NULL;
	current = new_head;
	current_src = head->next;
	while (current_src != NULL)
	{
		current->next = duplicate_token(current_src);
		if (current->next == NULL)
		{
			token_free(&new_head);
			return (NULL);
		}
		current = current->next;
		current_src = current_src->next;
	}
	return (new_head);
}

void parser(char *cmd_line, t_env *env) // a rajouter env quand expander fini
{
	t_token	*token_list;
	t_word	word;
	//t_token *dup_list;
	
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
	//printf("\n\n\nAFTER EXPANDER AND SYNTAX:\n");
	//token_print_amazing(&token_list);
	init_t_word(&word);
	if (define_word(&token_list, &word, env))
		return ;
	//printf("\n\n\nAFTER define word:\n");
	//token_print_amazing(&token_list);
	/* dup_list = duplicate_token_list(token_list);
	if (!dup_list)
		return ; */
	t_ast *ast = parse_expression(&token_list);
	printAST(ast,0);
	traverse_ast(ast);
	//syntax_check(); // Ultime check structure de l'input
	//ast(); // Tour de Controle de l'execution
	//printf("AFTER EXPANDER:\n");
	//token_print(&token_list);
	free_ast(ast);
	//token_free(&dup_list);
	token_free(&token_list);
}


