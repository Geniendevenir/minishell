/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcour_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:05:39 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/28 15:56:20 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_limiter
{
	char				*limit;
	int					quotes;// pas sure
	struct s_limiter	*next;
}		t_limiter;

typedef struct s_file
{
	int				fd;
	int				type;
	char			*data;
	struct s_file	*next;
}	t_file;


/* int exec_parent_node(t_ast *current, t_env *env)
{
	if (current->type == TOKEN_AND || current->type == TOKEN_OR)
		return (exec_operator(current, env));
	else if (is_command_or_builtin_or_abspath(current) == 1)
		return (exec_cmd(current, env));
	else if (current->type == TOKEN_PIPE)
		return (exec_pipe(current, env));
	else if (is_redirect_folder(current) == 1)
		return (exec_redirect(current));
	return (1);
} */

void traverse_ast(t_ast *root, t_env *env)
{
	t_ast *current;
	t_ast *last_visited;
	t_env *env2;

	env2 = env;
	free_env(env2);

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
			//exec_parent_node(current, env);
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