/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/08/17 19:07:28 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	TO DO
	1 - Executer une fois un builtin
	2 - Executer une fois un builtin ou non builtin
	3 - Executer en redirigeant
	4 - Executer plusieurs builtin ou cmd
	5 - Executer une fois un pipe
	6 - Executer plusieurs pipes
	7 - Expand HEREDOC
	8 - Error Management Execution
 */

//RECHECK TOUS LES RETURNS / EXIT STATUS


void	create_child(t_all *p, t_ast **token, int create_pipe)
{
	int	status;
    pid_t pid;

	status = 0;
	 if (create_pipe <= 0)
        return;
	pid = fork();
    if (pid < 0)
	{
		p->exit_status = -1;
        error_executer(NULL, 5);
        exit(EXIT_FAILURE); //Trouver un moyen de tout free avant d'exit
    }
    if (pid == 0)
	{
        // This is the child process
		if ((*token)->left)
			*token = (*token)->left;
		p->curr_pipe += 2;
        create_child(p, token, create_pipe - 1);  // Child forks another child
    }
	else
	{
        waitpid(pid, &status, 0);  // Wait for the child to complete
		if (WIFEXITED(status))
			p->exit_status = WEXITSTATUS(status);
	}
}

int		pipe_management(t_all *p, t_ast **token)
{
	t_ast	*current;
	int		pid1;
	int		pid2;

	current = *token;
	while (current->type == TOKEN_PIPE)
		p->max_pipe += 2;
	create_child(p, token, p->max_pipe / 2);
	
	//
	
	//fork
	//Si dans l'enfant return (0) = Continuer left_expand;
		//=> Se terminera quand on retournera a ce meme operateur et qu'on aura execute a gauche et a droite.
	//Si dans le parent waitpid puis return (1) = quitter left expand et remonter jusqu'au root
	//stock exit status
	return (0);
}

//printf("test\n");
t_ast *left_expand(t_all *p, t_ast *current)
{
	p->error = 0;
	while (current) //Down->Left : A CHAQUE DESCENTE LEFT OR RIHT EXPAND LES ENV POUR LES WORD ET LES HEREDOC DONT LE TYPE N'EST PAS SQ_LIMITER
	{
		/* if (current->type == TOKEN_PIPE)
		{
			p->error = pipe_management(p, &current);
			if (p->error == -2)
				return (current); //father goes back up after his son died 	
		} */
		if (current->value)
			printf("value = %s\n", current->value);
		if (current->type == TOKEN_PIPE)
		{
			p->max_pipe += 2;
			p->curr_pipe = p->max_pipe;
		}
		if (is_operator(current->type, 2))
			p->option = 0;
		if (current->state == STATE_WORD && current->type != WORD_SQLIMITER && current->type != WORD_LIMITER)
		{
			/* printf("current = %s\n", current->value);
			printf("current size = %ld\n", ft_strlen(current->value)); */
			p->error = split_word(p, &current);
			if (!current || !current->value)
				return (current);
			else if ((p->error != 0 && p->error != -1))
				return (current); //CHANGE RETURN AS IT ALSO TAKE INTO ACCOUNT EMPTY PROMTP AFTER EXPANDER
			else if (is_operator(current->type, 2) == 1)
				return (current);
		}
		if (p->error != -1)
		{
			if (!current->left)
				return (current);
			current = current->left;
		}
	}
	printf("test 9\n");
	return (current);
}

int		executer(t_all *p, t_ast *current, char **env)
{
	t_exec	exec;
	int		result;
	t_ast	*prev;

	printf("TURN:\n");
	exec_init(&exec);
	current = left_expand(p, current);
	printf("p->max_pipe = %d\n", p->max_pipe);
	printf("p->curr_pipe = %d\n", p->curr_pipe);
	if (p->error == 1)
	{
		exec_free(&exec);
		return (1);
	}
	while (current->parent && (current->parent->type == WORD_CMD || current->parent->type == WORD_OPTION)) //Up->Cmd
		current = current->parent;
	if (current && current->value && current->type == WORD_CMD)
	{
		if (get_command(current, &exec) == 1)
		{
			printf("Get Command Error Malloc\n");
			exec_free(&exec);
			return (1);
		}
		if (exec.command[0])
		{
			printf("CMD:\n");
			print_tab(exec.command);
			set_pipe(p, &exec);
			assign_redirect(current, &exec); //Add pipe
			p->exit_status = open_files(&exec);
			printf("RESULT:\n");
			if (is_builtin(exec.command[0]) == 1 && p->exit_status == 0)
				p->exit_status = exec_builtin(&p, exec.command, &p->env);
			else if (p->exit_status == 0)
			{
				p->exit_status = check_cmd(&exec, p->env);
				if (p->exit_status == 0)
				{
					printf("FULL CMD:\n");
					print_tab(exec.command);
					if (exec.path)
						printf("path = %s\n", exec.path);
					p->exit_status = exec_cmd(&exec, &p->exit_status, env);
					if (p->exit_status < 0)
					{
						write(2, "End child\n", 10);
						close_files(&exec, p->std_in, p->std_out);
						exec_free(&exec);
						return (1);
					}
				}
			}
			//if (p->exit_status == 1) stop
		}
	}
	//(ls || echo test) | $DONT: Fix dans check_syntax
	//ls && echo test < output.txt: Fix dans is_finished
	//ls | cat | cat
	close_files(&exec, p->std_in, p->std_out);
	exec_free(&exec);
	reset_pipe(p);
	/* while (current->parent)
	{
		if (is_operator(current->parent->type, 2) == 1)
			break ;
		current = current->parent;
	} */
	prev = current;
	printf("begining current = %s\n", prev->value);
	if (current->parent)
	{
		current = current->parent;
		printf("a\n");
		/* if (current->parent)
			printf("middle current->parent = %s\n", current->parent->value);
		if (current->right)
			printf("middle curr->right = %s\n", current->right->value); */
		
		while (current->parent && current->right == prev)
		{
			printf("b\n");	
			prev = current;
			current = current->parent;
		}
	}
	/* printf("ending current = %s\n", current->value);
	printf("curr->right = %s\n", current->right->value);
	printf("prev = %s\n", prev->value); */
	if (current->right == prev)
	{
		printf("end 1\n");	
		return (0);
	}
	current = prev;
	if (current->value)
		printf("current = %s\n", current->value);
	printf("p->exit_status = %d\n", p->exit_status);
	printf("p->option = %d\n", p->option);
	if (current->type == TOKEN_PIPE)
	{
		if (p->max_pipe > 0)
			p->curr_pipe--;
	}
	if (is_operator(current->type, 2) == 0 || p->option == 1)
	{
		current = get_next_operator(p, current, &prev);
		p->option = 0;
	}
	prev = current;
	if (current->type == TOKEN_OR && p->exit_status == 0)
	{
		if (current->parent && (current->type == TOKEN_OR || prev == current->right))
		{
			current = get_next_operator(p, current, &prev);
			if (prev == current)
				return (0);
		}
		while (current->parent && (current->type == TOKEN_OR || prev == current->right))
		{
			current = get_next_operator(p, current, &prev);
			prev = prev->parent;
		}
		if (current->type == TOKEN_OR || is_operator(current->type, 2) == 0)
			return (0);
		if ((!current->parent && p->option != 0) || (!current->parent && prev == current->right))
			return (0);
		p->option = 0;
	}
	else if (current->type == TOKEN_AND && p->exit_status != 0)
	{
		while (current->type == TOKEN_AND && current->parent)
		{
			current = get_next_operator(p, current, &prev);
			if (prev == current)
				return (0);
		}
		if (current->type == TOKEN_AND)
			return (0);
		if ((!current->parent && p->option != 0))
			return (0);
		p->option = 0;
	}
	if ((!current->parent && p->option != 0) || is_operator(current->type, 2) == 0)
	{
		printf("end 2\n");
		return (0);
	}
	else if (p->option == 1)
		current = get_next_operator(p, current, &prev);
	if (current->right)
	{
		p->option = 1;
		printf("\n\n");
		executer(p, current->right, env);
	}
	printf("end 3\n");
	return (0);
}

t_ast *get_next_operator(t_all *p, t_ast *current, t_ast **prev)
{
	if (current->parent)
	{
		current = current->parent;
		if (current->type == TOKEN_PIPE)
		{
			if (p->max_pipe > 0)
				p->curr_pipe--;
		}
	}
	while (current->parent)
	{
		if (is_operator(current->type, 2))
			break ;
		current = current->parent;
		if (current->type == TOKEN_PIPE)
		{
			if (p->max_pipe > 0)
				p->curr_pipe--;
		}
	}
	return (current);
}

