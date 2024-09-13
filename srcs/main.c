/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/13 14:59:40 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sig_int = 0;

void	testAST(t_ast* node, int option)
{
	if (option == 1)
	{
		while (node->left)
			node = node->left;
		printf("last left = %s\n", node->value);
		while (node->parent)
		{
			printf("parent = %s\n", node->parent->value);
			node = node->parent;
		}
	}
	else if (option == 2)
	{
		while (node->right)
			node = node->right;
		while (node->left)
			node = node->left;
		printf("last left right = %s\n", node->value);
		while (node->parent)
		{
			printf("parent = %s\n", node->parent->value);
			node = node->parent;
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_all	*p;

	(void)argc;
	(void) **argv;
	p = init_all(env);
	if (!p || p == NULL)
		return (EXIT_FAILURE);
	while (p->line == NULL)
	{
		minishell(p, env);
	}
	return (0);
}

char	*minishell(t_all *p, char **env)
{
	extern int	sig_int;
	t_ast 	*current;

	rl_event_hook = event;
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (p->line == NULL)
	{
		printf("exit\n");
		p->exit_status = 0;
		return (free(p->line), free_all(p), rl_clear_history(), exit(0), NULL);
	}
	//printf("sigquit = %d\n", p->sig->sig_quit);
	if (p->sig->sig_quit == 0  && skip_whitespace(p->line))
	{
		p->error = parser(p->line, p->env, &p->ast, &p);
		//printf("next_status = %d\n", next_status);
		//printAST(p->ast, 0);
		if (p->error == 0)
		{
			printAST(p->ast, 0);
			testAST(p->ast, 1);
			testAST(p->ast, 2);
			current = p->ast;
			if (executer(p, current, env) == 1)
			{
				//printf("executor ended\n");
				free_ast(p->ast);
				return (free(p->line), free_all(p), rl_clear_history(), exit(0), NULL);
			}
			free_here_docs(p->here_doc);
			free_ast(p->ast);
		}
		add_history(p->line);
	}
	/* if (p->line)
	{
		write(2, "p->line = ", 10);
		write(2, p->line, ft_strlen(p->line));
		write(2, "\n", 1);
	} */
	sig_int = 0;
	free(p->line);
	p->line = NULL;
	p->line_num++;
	return (p->line);
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=./.readline.supp --trace-children=yes ./minishell
//valgrind -suppressions=./.readline.supp ./minishell
//ps -f --forest : see shell process tree
