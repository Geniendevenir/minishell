/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:24:04 by allan             #+#    #+#             */
/*   Updated: 2024/07/10 10:17:54 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*

TO DO:
-Finir / Corriger AST OK
-Builtins 
-Check Leak
-Revoir Wildcard avec lstat
-Error Management de TOUT le code: OK
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

//printf("test\n");
int parser(char *cmd_line, t_env *env, t_ast **ast, t_all **p)
{
	t_token	*token_list;
	t_word	word;
	int		error;
	int		sub_shell;
	//t_token *dup_list;
	
	error = 0;
	if (cmd_line[0] == '\0')
		return (1);
	token_list = malloc(sizeof(t_token));
	if (!token_list)
		return (1);
	token_init(&token_list);
	if (lexer(cmd_line, &token_list, error) == 1)
		return (1);
/* 	printf("AFTER LEXER:\n");
	token_print(&token_list);
	token_print_amazing(&token_list); */
	if (check_syntax(token_list) == 1)
	{
		token_free(&token_list);
		return (1);
	}
	//here_doc(&token_list,p);
	//printf("TESTT APRES HEREDOC\n");
	init_t_word(&word);
	if (define_word(&token_list, &word))
		return (1);
	/* printf("\n\nAFTER define word:\n");
	token_print_amazing(&token_list); */
	sub_shell = 0;
	*ast = parse_expression(&token_list, sub_shell);
	//printAST(*ast, 0);
	//printf("AFTER EXPANDER:\n");
	//token_print(&token_list);
	//token_free(&dup_list);
	token_free(&token_list);
	return (0);
}
