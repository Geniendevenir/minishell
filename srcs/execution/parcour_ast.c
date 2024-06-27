/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcour_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:05:39 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/27 19:16:59 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_limiter
{
	char				*limit;
	int					quotes;// pas sure
	struct s_limiter	*next;
}		t_limiter;

typedef struct s_table {
	int			left_state;
	int			right_state;
	int			exit_state;
	enum s_type	type_node;
	char		*value;
	char		**options;
	
}	t_table;

void traverse_ast(t_ast *root)
{
	t_ast *current;
	t_ast *last_visited;

	current = root;
	last_visited = NULL;
	while (current)
	{
		while (current->left)
		{
			current = current->left;
			if (current->left == NULL)
				printf("Visited left node  with value: %s\n", current->value);
		}
		while (current->parent && (current->right == NULL || current->right == last_visited))
		{
			last_visited = current;
			current = current->parent;
			printf("Visited parent node with value: %s\n", current->value);
		}
		if (current->right && current->right != last_visited)
		{
			current = current->right;
			printf("Visited right node with value: %s\n", current->value);
		}
		else
			break; // Si on ne peut plus aller à droite, on a fini le parcours
	}
}