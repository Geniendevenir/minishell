/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/07/11 17:11:08 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
	NE PAS OUBLIER
	1 - Expand HEREDOC
	2 - 
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
        create_child(p, create_pipe - 1, token);  // Child forks another child
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

	current = token;
	while (current->type == TOKEN_PIPE)
		p->max_pipe += 2;
	create_child(p, p->max_pipe / 2, token);
	
	//
	
	//fork
	//Si dans l'enfant return (0) = Continuer left_expand;
		//=> Se terminera quand on retournera a ce meme operateur et qu'on aura execute a gauche et a droite.
	//Si dans le parent waitpid puis return (1) = quitter left expand et remonter jusqu'au root
	//stock exit status
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

int		executer(t_all *p) //, int option
{
	t_ast *current;
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
		//EXECUTE CMD
	}
	exec_free(&exec);
	while (current->parent)
	{
		if (is_operator(current->type, 2))
			executer(p);
		current = current->parent;
	}
	
	//remonter->parent et if (is_operator)
	//recursif(option 2);
	exec_free(&exec);
	printAST(p->ast, 0);
	return (0);
}

	/*
	printf("new in = %s\n", current->value);
	printf("new out = %s\n", current->value);
	printf("final node = %s\n", current->value);
	 */
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

/* int		executer(t_ast **ast, t_env *env, int *exit_status)
{
	t_ast *current;
	t_exec	exec;
	int		result;

	current = *ast;
	exec_init(&exec);
	while (current)
	{
		if (current->state == STATE_WORD)
		{
			if (split_word(ast, current, env) != 0)
			{
				exec_free(&exec);
				free_ast(*ast);
				return (1);
			}
		}
		if (!current->left)
			break ;
		current = current->left;
	}
	exec_free(&exec);
	free_ast(*ast);
	return (0);
} */