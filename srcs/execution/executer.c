/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/07/22 18:01:35 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	TO DO
	1 - Expand HEREDOC
	2 - Executer une fois un builtin
	3 - Executer une fois un builtin ou non builtin
	4 - Executer plusieurs builtin ou cmd
	5 - Executer une fois un pipe
	6 - Executer plusieurs pipes
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
		if (current->state == STATE_WORD && current->type != WORD_SQLIMITER && current->type != WORD_LIMITER)
		{
			printf("current = %s\n", current->value);
			printf("current size = %ld\n", ft_strlen(current->value));
			p->error = split_word(p, &current);
			if (p->error != 0 && p->error != -1)
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

int		executer(t_all *p)
{
	t_ast	*current;
	t_exec	exec;
	int		result;

	current = p->ast;
	exec_init(&exec);
	current = left_expand(p, current);
	if (p->error == 1)
	{
		exec_free(&exec);
		return (1);
	}
	// II - TROUVER CMD/ FIND REDIRECT / CREATE CMD / EXECUTE COMMAND
	if (current)
	{
		while (current->parent && (current->parent->type == WORD_CMD || current->parent->type == WORD_OPTION)) //Up->Cmd
			current = current->parent;
		if (get_command(current, &exec) == 1) // Get cmd
		{
			printf("Get Command Error Malloc\n");
			exec_free(&exec);
			return (1);
		}
		// PERFECT
		/* 
		if (p->curr_pipe == p->max_pipe)
			set_pipe(&exec, 1); //pipe gauche
		else if (p->curr_pipe == 1)
			set_pipe(&exec, 3); //pipe milieu
		else
			set_pipe(&exec, 2); //pipe droite */
		//set_pipe(&exec, 1); add rule for 'in between pipe'
		
		//
		assign_redirect(current, &exec); //Assign Redirect Finir check pipe
		p->exit_status = open_files(p, &exec);
		if (p->exit_status != 0)
		{
			exec_free(&exec);
			return (p->exit_status); // Peut return void si besoin nqiue norminette
		}
		p->exit_status = check_cmd(&exec, p->env);
		if (p->exit_status != 0)
		{
			close_files(&exec);
			exec_free(&exec);
			if (p->exit_status == 1)
				error_executer(NULL, 4);
			return (1); // Peut return void si besoin nqiue norminette
		}
		printf("COMMAND:\n");
		print_tab(exec.command);
		printf("RESULT:\n");
		exec_builtin(exec.command);
		//EXECUTE CMD
	}
	exec_free(&exec);
	/* while (current->parent)
	{
		if (is_operator(current->type, 2))
			executer(p);
		current = current->parent;
	} */
	
	//remonter->parent et if (is_operator)
	//recursif(option 2);
	return (0);
}
