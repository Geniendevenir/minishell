/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 12:50:38 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

t_sig	g_sig;

char	*minishell(t_all *p)
{
	rl_event_hook = event;
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (p->line == NULL)
	{
		printf("exit\n");
		return (free(p->line), free_all(p), rl_clear_history(), exit(0), NULL);
	}
	if (p->sig->sig_int == 0)
	{
		parser(p->line, p->env, &p->ast);
		/* if (parser(p->line, p->env, &p->ast) == 0)
		{
			printAST(p->ast,0);
			executer(&p->ast, p->env);
			free_ast(p->ast);
		} */
		add_history(p->line);
	}
	p->sig->sig_int = 0;
	free(p->line);
	p->line = NULL;
	return (p->line);
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=./.readline.supp ./minishell
//ps -f --forest : see shell process tree

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
		minishell(p);
	}
	return (EXIT_SUCCESS);
}

//TEST MAIN
/*	TEST EXIT 
	int exit_status;
	
	if (argc < 3)
		return (1);
	exit_status = ft_exit(argv + 2, 1);
	printf("exit_status = %d\n", exit_status);
	return (1); */

	
	/* TEST EXPORT
	
	t_env	*env_list;
	int error;
	
	if (argc < 3)
		return (1);
	env_list = env_to_struct(env);
	printf("ORIGINAL ENV\n\n");
	print_env(env_list);
	error = ft_export(argv + 2, &env_list);
	if (error == 1)
	{
		free_env(env_list);
		return (1);
	}
	printf("NEW ENV\n\n");
	print_env(env_list);
	printf("return export = %d\n", error);
	free_env(env_list);
	return (0); */
	
	/* TEST CD
	
	if (argc != 3)
		return (1);
	int error;
	error = ft_cd(*(argv + 2));
	printf("return cd = %d\n", error);
	return (0); */