/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/10 22:45:29 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		sig_int = 0;

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
	return (0);
}



char	*minishell(t_all *p)
{
	extern int	sig_int;

	rl_event_hook = event;
	p->line = readline("\033[1;032mMinishell> \033[m");
	if (p->line == NULL)
	{
		printf("exit\n");
		p->exit_status = 0;
		return (free(p->line), free_all(p), rl_clear_history(), exit(0), NULL);
	}
	//printf("sigquit = %d\n", p->sig->sig_quit);
	if (sig_int == 0 && p->sig->sig_quit == 0)
	{
		p->exit_status = parser(p->line, p->env, &p->ast, &p);
		//printf("next_status = %d\n", next_status);
		/* if (p->exit_status == 0)
		{
			p->exit_status = executer(p);
			free_ast(p->ast);
		} */
		printAST(p->ast, 0);
		free_ast(p->ast);
		add_history(p->line);
	}
	sig_int = 0;
	free(p->line);
	p->line = NULL;
	p->line_num++;
	return (p->line);
}

//valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=./.readline.supp ./minishell
//ps -f --forest : see shell process tree


//TEST MAIN

/* 		TRUE MAIN

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
return (EXIT_SUCCESS); */

/* 	TEST PWD
	int exit_status;
	
	if (argc < 2)
		return (1);
	exit_status = ft_pwd(argv[2]);
	printf("exit_status = %d\n", exit_status);
	return (1); */
	
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
	
	/* TEST ECHO 
	
	int exit_status;
	
	if (argc < 2)
		return (1);
	exit_status = ft_echo(argv + 2); //commence a la premiere option
	printf("exit_status = %d\n", exit_status);
	return (0);
 */