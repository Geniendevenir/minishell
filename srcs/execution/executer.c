/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/07/06 18:29:35 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exec	exec_init(t_exec exec)
{
	exec.pipe = 0;
	exec.redirectin = 0;
	exec.redirectout = 0;
	exec.in = NULL;
	exec.out = NULL;
	exec.command = NULL;
	return (exec);
}

void	exec_free(t_exec *exec)
{
	exec->pipe = 0;
	exec->redirectin = 0;
	exec->redirectout = 0;
	exec->in = NULL;
	exec->out = NULL;
	if (exec->command)
		free_array(exec->command);
	exec->command = NULL;
}

int		executer(t_ast **ast, t_env *env, int *exit_status)
{
	t_ast *current;
	t_exec	exec;
	
	current = *ast;
	while (current && current->left)
		current = current->left;
	//printf("%s\n", current->value); //OK
	while (current && current->parent && (current->type != WORD_CMD || current->type != WORD_OPTION))
		current = current->parent;
	printf("%s\n", current->value); // OK
	if (current->type != WORD_CMD)
		current = current->left;
	if (get_command(current, &exec) == 1)
	{
		printf("Get Command Error Malloc\n");
		exec_free(&exec);
		return (1);
	}
	printf("RESULT\n");
	print_tab(exec.command);
	exec_free(&exec);
	/* while (current && current->parent && is_operator(current->type, 2) == 0)
	{
		current = current->parent;
	} */
	return (0);
}

int get_command(t_ast *current, t_exec *exec)
{
	int	size;
		
	size = command_size(current);
	printf("%d\n", size);
	exec->command = parse_command(current, size);
	if (!exec->command)
		return (1);
	return (0);
}

int	command_size(t_ast *current)
{
	int size;

	size = 1;
	while (current->left && (current->type == WORD_CMD || current->type == WORD_OPTION))
	{
		size++;
		current = current->left;
	}
	return (size);
}

char **parse_command(t_ast *current, int size)
{
	char **command;
	int i;

	i = 0;
	command = malloc(sizeof(char *) * size + 1);
	if (!command)
		return (NULL);
	while (size > 0)
	{
		if (!current->value)
			return (NULL);
		printf("%s\n", current->value);
		command[i] = ft_strdup(current->value); //pas besoin de free current->value ici
		if (!command[i])
		{
			free_array(command);
			return (NULL);
		}
		i++;
		size--;
		current = current->left;
	}
	command[i] = 0;
	return (command);
}


/*
((> output-1.txt echo hello > output.txt lol && echo world > output2.txt > output3.txt) > output4.txt && < input.txt < input2.txt cat | > output5.txt grep hello > output6.txt) && env > final_output.txt




((< input.txt cat  > output.txt > output2.txt) > output4.txt && (echo world && (cat -e | grep hello) || echo test) > output2.txt)


typedef struct s_ast {
	int subshell;
	int exit_state;
	enum s_type type;
	enum s_state state;
	char		*value;
	struct s_ast *left;
	struct s_ast *right;
	struct s_ast *parent;
}				t_ast;

	typedef struct s_exec
{
	bool	pipe;
	bool	redirectin;
	bool	redirectout;
	struct s_ast *in;
	struct s_ast *out;
	char **command;
}				t_exec;


													ALGO REDIRECTION RADAR ASCENDANT
	Une fois que la commande est trouve
	current = La bonne commande
	search_redirect = current;
	bool redirectin = 0;
	bool redirectout = 0;
	while (search_redirect->parent && (exec->redirectin == 0 || exec->redirectout == 0))
	{
		if (is_operator(search_redirect) == 1 && exec->pipe == 0) //En remontant Si je croise un pipe, lors du reset de struct exec, pipe = 1;
		{
			if (exec->in != NULL)
				exec->redirectin = 1;
			if (exec->out != NULL)
				exec->redirectout = 1;
		}
		if (search_redirect->type == TOKEN_PIPE && exec->pipe == 1)
			exec->pipe == 0;
		else if (search_redirect->type == TOKEN_REDIRECTIN)
			exec->in = search_redirect;
		else if (search_redirect->type == TOKEN_REDIRECTOUT)
			exec->out = search_redirect;
		search_redirect = search_redirect->parent;
	}



													ALGO COMMANDE RADAR DESCENDANT
	On est tout en bas d'une branche
	while (current->parent && current->type != TOKEN_COMMAND)
		current = current->parent
	if (current->type == TOKEN_COMMAND)
	{
		if (get_command(current, exec_command) == 1)
			return (1);
	}

	int get_command(t_ast *current, t_exec *exec)
	{
		int	size;
		
		size = command_size(current);
		exec->command = parse_command(current, size);
		if (!command)
			return (1);
		return (0);
	}

	int	command_size(t_ast *current)
	{
		int size;

		size = 0;
			while (current->next && (current->type == COMMAND || current->type == OPTION)
			{
				size++;
				current = current->next;
			}
		return (size)
	}

	char **parse_command(t_ast *current, int size)
	{
		char **command;
		int i;

		i = 0;
		command = malloc(sizeof(char *) * size + 1);
		if (!command)
			return (NULL);
		while (current->next && size != 0)
		{
			if (!current->value)
				return (NULL)
			command[i] = ft_strdup(current->value); //pas besoin de free current->value ici
			if (!command[i])
			{
				free_cmd(command);
				return (NULL);
			}
			size--;
			current = current->next;
		}
		command[i] = 0;
		return (command);
	}



	
	INFO DESCENDANTE:
		-Subshell
		-full commande (Radar descendant)

	INFO ASCENDANTE:
		-Redirection (Radar ascendant)
		-Commande
		-Operateur

	if (current->type == )

		PRE-SHOT EXECUTION
		int		subshell = 0;
		
		1 - While (current->left)
		{
			if (current->state == TOKEN_OPERATOR && current->subshell != 0)
				
			current = current->left
		}
*/