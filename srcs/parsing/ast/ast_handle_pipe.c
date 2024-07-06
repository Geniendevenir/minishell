/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:38:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/06 13:23:56 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	if_no_last_ope(t_ast **new_node, t_ast_ptr **list)
{
	if ((*list)->last_pipe)
	{
		(*new_node)->left = (*list)->last_pipe;
		if ((*list)->root && ((*list)->root)->type
			!= TOKEN_AND && ((*list)->root)->type != TOKEN_OR)
			(*list)->root = *new_node;
	}
	else
	{
		(*new_node)->left = (*list)->root;
		(*list)->root = *new_node;
	}
	(*list)->current = *new_node;
	(*list)->last_pipe = *new_node;
}

void	if_last_ope_exist(t_ast **new_node, t_ast_ptr **list)
{
	(*new_node)->left = (*list)->last_ope->right;
	(*list)->last_ope->right = *new_node;
	(*new_node)->parent = (*list)->last_ope;
	(*list)->current = *new_node;
	(*list)->last_pipe = *new_node;
}

/*handle_pipe: add a PIPE node in the ast tree,
PIPE has the second priority compared to the ROOT
EXAMPLE:
   ROOT (&& and ||)
			|
		   PIPE
		    |
		 REDIRECTIONS/FILES
		 	|
		   CMD
		    |
		 OPTIONS
		    |
		 BRANCH
So we have to save the address of the last PIPE in
"last_pipe" for handle REDIRECTIONS.
SPECIAL CASE:
-No OPERATOR before PIPE:
if it's the first pipe we have to put on the left of the pipe the root
, because we have only that before
otherwise we have to put save_pipe.
-An OPERATOR before PIPE:
we have to put on the left of the last OPERATOR

*/
void	handle_pipe(t_token **tokens, t_ast_ptr **list, int sub_shell)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node(*tokens, sub_shell);
	if (!new_node)
		return ;
	if (!(*list)->last_ope)
		if_no_last_ope(&new_node, list);
	else
		if_last_ope_exist(&new_node, list);
	free_token_and_next_in_ast(tokens, &temp);
}
