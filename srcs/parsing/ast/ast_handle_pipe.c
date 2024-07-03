/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_handle_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 20:38:03 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/03 13:29:14 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	if_is_pipe(t_token **tok)
{
	//(*tok)->type == TOKEN_PIPE || 
	if ((*tok)->type == TOKEN_PIPE)
		return (1);
	else
		return (0);
}

void	while_in_handle_pipe(t_ast **current, t_ast **new_node, t_ast *save_operator)
{
	while ((save_operator != (*current)) &&((*current)->parent && *current))
		*current = (*current)->parent;
	(*new_node)->left = (*current)->right;
	(*new_node)->parent = *current;
	(*current)->right = (*new_node);
	while (*current && (*current)->right)
		(*current) = (*current)->right;
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
void	handle_pipe(t_token **tokens, t_ast **current, t_ast **root, t_ast **save_operator, t_ast **save_pipe)
{
	t_ast	*new_node;
	t_token	*temp;

	new_node = create_node((*tokens)->type, (*tokens)->value);
	if (!new_node)
		return ;
	printf("IN PIPE :\n");
	if (!*save_operator)
	{
			printf("in if not operator :");
			if(*save_pipe)
			{
				new_node->left = *save_pipe;
				if(*root &&(*root)->type != TOKEN_AND && (*root)->type != TOKEN_OR)
					*root = new_node;
				printf("in save_pipe  new_node->left = %s\n", new_node->left->value);
			}
			else
			{	
				new_node->left = *root;
				*root = new_node;
				printf("in else root new_node->left = %s\n", new_node->left->value);
			}
			//*save_operator = new_node;
			*current = new_node;
			*save_pipe = new_node;
			printf("save_pipe address in PIPE = %p | current address = %p\n", save_pipe, current);
	}
	else
	{
		//(*root)->left = new_node;
		//printf("root->value %s | save_operator = %s", (*root)->value, (*save_operator)->value);
		new_node->left = (*save_operator)->right;
		(*save_operator)->right = new_node;
		new_node->parent = *save_operator;
		//*save_operator = new_node;
		*current = new_node;
		//*save_operator = *current;
		*save_pipe = new_node;
		printf("IN ELSE save_pipe address in PIPE = %p\n", save_pipe);
	}
	printf("\n");
	free_token_and_next_in_ast(tokens, &temp);
}
