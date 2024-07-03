/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:38:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 18:32:56 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_pipe(t_token **tok)
{
	if ((*tok)->type == TOKEN_PIPE)
		return (1);
	else
		return (0);
}

void	while_in_handle_pipe(t_ast **current, t_ast **new_node,
	t_ast *save_operator)
{
	while ((save_operator != (*current)) && ((*current)->parent && *current))
		*current = (*current)->parent;
	(*new_node)->left = (*current)->right;
	(*new_node)->parent = *current;
	(*current)->right = (*new_node);
	while (*current && (*current)->right)
		(*current) = (*current)->right;
}

void	if_no_last_operator(t_ast **current, t_ast **new_node,
	t_ast **save_operator, t_ast **save_pipe)
{
	(*new_node)->left = (*save_operator)->right;
	(*save_operator)->right = *new_node;
	(*new_node)->parent = *save_operator;
	*current = *new_node;
	*save_pipe = *new_node;
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
void	handle_pipe(t_token **tokens, t_ast **current,
	t_ast **root, t_ast **last_operator, t_ast **last_pipe)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	if (!*last_operator)
	{
			if (*last_pipe)
			{
				new_node->left = *last_pipe;
				if (*root &&(*root)->type != TOKEN_AND && (*root)->type != TOKEN_OR)
					*root = new_node;
			}
			else
			{	
				new_node->left = *root;
				*root = new_node;
			}
			*current = new_node;
			*last_pipe = new_node;
	}
	else
		if_no_last_operator(current, &new_node, last_operator, last_pipe);
	free_token_and_next_in_ast(tokens, &temp);
}
