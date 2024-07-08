/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/07/08 17:02:16 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//RECHECK TOUS LES RETURNS / EXIT STATUS
int		execute_cmd(t_exec	*exec, t_env *env)
{
	char	*path;
	char	*temp;
	int		error;

	error = 0;
	temp = NULL;
	if (!exec->command || !exec->command[0])
		return (1);
	//test builtin
	//printf("exec->command[0] = %s\n", exec->command[0]);
	if (access(exec->command[0], X_OK) == -1) //check if not absolute path
	{
		path = get_path(exec->command[0], env, &error);
		if (error != 0)
		{
			printf("ERROR: get_path error\n");
			if (path)
				free (path);
			return (1); //malloc error
		}
		if (!path)
		{
			write(1, exec->command[0], ft_strlen(exec->command[0]));
			write(1, ": command not found\n", 20);
			return (127);
		}
		temp = exec->command[0];
		free(temp); // Ca sert a qqchose ?
		exec->command[0] = path;
	}
	return (0);
}

int		executer(t_ast **ast, t_env *env, int *exit_status)
{
	t_ast *current;
	t_exec	exec;
	int		result;

	current = *ast;
	exec_init(&exec);
	while (current && current->left) //Down->Left : A CHAQUE DESCENTE LEFT OR RIHT EXPAND LES ENV POUR LES WORD ET LES HEREDOC DONT LE TYPE N'EST PAS SQ_LIMITER 
		current = current->left;
	while (current->parent && (current->parent->type == WORD_CMD || current->parent->type == WORD_OPTION || current->parent->type == WORD_BUILTIN)) //Up->Cmd
		current = current->parent;
	if (get_command(current, &exec) == 1) // Get cmd
	{
		printf("Get Command Error Malloc\n");
		exec_free(&exec);
		return (1);
	}
	printf("COMMAND:\n");
	print_tab(exec.command);
	printf("REDIRECT\n");
	//set_pipe(&exec, 1);
	assign_redirect(current, &exec); //Assign Redirect Finir check pipe
	result = execute_cmd(&exec, env); //execute
	if (result != 0)
	{
		exec_free(&exec);
		if (result == 1)
			printf("execute_cmd Malloc Error\n");
		return (1);
	}
	printf("COMMAND:\n");
	print_tab(exec.command);
	exec_free(&exec);
	return (0);
}


int		assign_redirect(t_ast *current, t_exec *exec)
{
	while (current && (exec->redirectin == 0 || exec->redirectout == 0))
	{
		if (is_operator(current->type, 2) == 1) //En remontant Si je croise un pipe, pipe 1 = gauche/ pipe 2 = millieu / pipe 3 = droite
		{
			if (current->type == TOKEN_PIPE)
			{
				if (exec->pipe == 1)
				{
					if (exec->out == NULL)
					{
						exec->out = current;
						exec->redirectout = 1;
					}
					if (exec->in != NULL)
					exec->redirectin = 1;
				}
				else if (exec->pipe == 3)
				{
					if (exec->in == NULL)
					{
						exec->in = current;
						exec->redirectin = 1;
					}
					if (exec->out != NULL)
					exec->redirectout = 1;
				}
			}
			else
			{
				if (exec->in != NULL)
					exec->redirectin = 1;
				if (exec->out != NULL)
					exec->redirectout = 1;
			}
		}
		if ((current->type == WORD_FILEIN || current->type == WORD_LIMITER) && exec->redirectin == 0)
		{
			printf("new in = %s\n", current->value);
			exec->in = current;
		}
		else if ((current->type == WORD_FILEOUT || current->type == WORD_FILEOUT_APPEND) && exec->redirectout == 0)
		{
			printf("new out = %s\n", current->value);
			exec->out = current;
		}
		if (!current->parent)
			break;
		current = current->parent;
	}
	printf("final node = %s\n", current->value);
	if (exec->in)
		printf("in = %s\n", exec->in->value);
	else
		printf("in = NULL\n");
	if (exec->out)
		printf("out = %s\n", exec->out->value);
	else
		printf("out = NULL\n");
	return (0);
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