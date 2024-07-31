/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/08/01 00:02:52 by allan            ###   ########.fr       */
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
		if (is_operator(current->type, 2))
			p->option = 0;
		if (current->state == STATE_WORD && current->type != WORD_SQLIMITER && current->type != WORD_LIMITER)
		{
			/* printf("current = %s\n", current->value);
			printf("current size = %ld\n", ft_strlen(current->value)); */
			p->error = split_word(p, &current);
			if (current->value == NULL)
				return (current);
			if ((p->error != 0 && p->error != -1))
				return (current); //CHANGE RETURN AS IT ALSO TAKE INTO ACCOUNT EMPTY PROMTP AFTER EXPANDER
		}
		if (p->error != -1)
		{
			if (!current->left)
				return (current);
			current = current->left;
		}
	}
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
	if (p->error == 1)
	{
		exec_free(&exec);
		return (1);
	}
	while (current->parent && (current->parent->type == WORD_CMD || current->parent->type == WORD_OPTION)) //Up->Cmd
		current = current->parent;
	if (current && current->value)
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
			/* 
			if (p->curr_pipe == p->max_pipe)
				set_pipe(&exec, 1); //pipe gauche
			else if (p->curr_pipe == 1)
				set_pipe(&exec, 3); //pipe milieu
			else
				set_pipe(&exec, 2); //pipe droite */
			//set_pipe(&exec, 1); add rule for 'in between pipe'
			assign_redirect(current, &exec); //Assign Redirect Finir check pipe
			p->exit_status = open_files(&exec);
			if (p->exit_status == 1)
			{
				exec_free(&exec);
				return (p->exit_status); // Peut return void si besoin nqiue norminette
			}
			if (is_builtin(exec.command[0]) == 1)
				p->exit_status = exec_builtin(exec.command);
			else
			{
				p->exit_status = check_cmd(&exec, p->env);
				/* if (p->exit_status == 1 || p->exit_status == 127)
				{
					close_files(&exec, p->std_in, p->std_out);
					exec_free(&exec);
					if (p->exit_status == 1)
						error_executer(NULL, 4);
					return (1); // Peut return void si besoin nqiue norminette
				} */
				if (p->exit_status == 0)
				{
					printf("FULL CMD:\n");
					print_tab(exec.command);
					if (exec.path)
						printf("path = %s\n", exec.path);
					p->exit_status = exec_cmd(&exec, &p->exit_status, env);
					printf("p->exit_status = %d\n", p->exit_status);
				}
			}
			//if (p->exit_status == 1) stop
		}
	}
	/* if (current->value)
		printf("current->value = %s\n", current->value); */
	close_files(&exec, p->std_in, p->std_out);
	exec_free(&exec);
	prev = NULL;
	//current = get_next_operator(current, &prev);
	prev = current;
	if (current->parent)
	{
		current = current->parent;
		while (current->parent && current->right == prev)
		{
			prev = current;
			current = current->parent;
		}
	}
	if (current->right == prev)
		return (0);
	current = prev;
	if (is_operator(current->type, 2) == 0)
		current = get_next_operator(current, &prev);
	prev = current;
	if (current->type == TOKEN_OR && p->exit_status == 0 && p->option == 0)
	{
			current = get_next_operator(current, &prev);
			if (prev == current)
				return (0);
			p->option = 0;
	}
	else if (current->type == TOKEN_AND && p->exit_status != 0 && p->option == 0)
	{
			current = get_next_operator(current, &prev);
			if (prev == current)
				return (0);
			p->option = 0;
	}
	if ((!current->parent && p->option != 0) || is_operator(current->type, 2) == 0)
		return (0);
	else if (p->option == 1)
		current = get_next_operator(current, &prev);
	if (current->right)
	{
		p->option = 1;
		printf("\n\n");
		executer(p, current->right, env);
	}
	return (0);
}

t_ast *get_next_operator(t_ast *current, t_ast	**prev)
{
	if (current->parent)
	{
		//*prev = current;	
		current = current->parent;
	}
	while (current->parent)
	{
		if (is_operator(current->type, 2))
			break ;
		//*prev = current;
		current = current->parent;
	}
	return (current);
}

/*
	if (is_operator(current->right->type, 2))
		executer(p, current->right, env);
	else
	{
		p->option = 1;
		executer(p, current->right, env);
	}
*/
