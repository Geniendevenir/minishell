/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:48:02 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 19:21:10 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "../../../inc/minishell.h"

typedef struct s_ast_ptr
{
	t_ast	*root;
	t_ast	*current;
	t_ast	*last_ope;
	t_ast	*last_pipe;
}	t_ast_ptr;


void	init_pointer_astptr(t_ast_ptr **list)
{
	*list = malloc(sizeof(t_ast_ptr)); // allouer de la mémoire pour list
	if (!*list)
	{
		printf("erreur malloc\n");
		return ;
	}
	(*list)->last_pipe = NULL;
	(*list)->last_ope = NULL;
	(*list)->root = NULL;
	(*list)->current = NULL;
}

void	modif(t_ast_ptr **list)
{
	(*list)->last_pipe = create_node(TOKEN_WORD, "42");
	
}
void	modif2(t_ast_ptr **list)
{
	(*list)->last_pipe = create_node(TOKEN_WORD, "43");
	
}
int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void) **argv;
	(void) **env;
	t_ast_ptr *list;
	
	init_pointer_astptr(&list);
	modif(&list);
	if((list)->last_pipe)
		printf("list->last_pipe->value) = %s\n", list->last_pipe->value);
	else
		printf("loser\n");
	modif2(&list);
	if((list)->last_pipe)
		printf("list->last_pipe->value) = %s\n", list->last_pipe->value);
	else
		printf("loser\n");
	
	return (1);
} */