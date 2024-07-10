/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:23:21 by allan             #+#    #+#             */
/*   Updated: 2024/07/10 17:48:42 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	TO DO
	1 - Separate function split word OK
	2 - Find a way to Include EXIT STATUS OK
	3 - CREATE THE ExIT_STATUS MAP
	3 - Finish left_expand: Subshell, Exit Status, Set Pipe
	4 - Theorize an algo to explore and execute the entire ast
 */

/* int		create_subshell(t_all *p, t_ast *current, int subshell)
{
	if (is_operator(current->type, 1) == 1 && current->subshell > subshell)
	{
		//find output redirect of the whole subshell
		//open folders (Look at the permission depending on < or <<)
		//fork
		//Si dans l'enfant return (0) = Continuer left_expand;
			//=> Se terminera quand on retournera a ce meme operateur et qu'on aura execute a gauche et a droite.
		//Si dans le parent waitpid puis return (1) = quitter left expand et remonter jusqu'au root
		//stock exit status
	}
	else if (current->type == TOKEN_PIPE)
	{
		
	}
} */

//printf("test\n");
t_ast *left_expand(t_all *p, t_ast *current)
{
	p->error = 0;
	while (current) //Down->Left : A CHAQUE DESCENTE LEFT OR RIHT EXPAND LES ENV POUR LES WORD ET LES HEREDOC DONT LE TYPE N'EST PAS SQ_LIMITER
	{
		//if (current->subshell == 1) fork
		
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

int		executer(t_all *p)
{
	t_ast *current;
	t_exec	exec;
	int		result;

	current = p->ast;
	exec_init(&exec);
	//printf("test\n");
	//while(current->parent);
	current = left_expand(p, current);
	printf("p->error = %d\n", p->error);
	if (p->error == 1)
	{
		exec_free(&exec);
		return (1);
	}
	/* if (!current)
		//exit; */
	if (current)
	{
		while (current->parent && (current->parent->type == WORD_CMD || current->parent->type == WORD_OPTION || current->parent->type == WORD_BUILTIN)) //Up->Cmd
			current = current->parent;
		if (get_command(current, &exec) == 1) // Get cmd
		{
			printf("Get Command Error Malloc\n");
			exec_free(&exec);
			return (1);
		}
		// PERFECT

		printf("REDIRECT\n");
		//set_pipe(&exec, 1); add rule for 'in between pipe'
		assign_redirect(current, &exec); //Assign Redirect Finir check pipe
		result = check_cmd(&exec, p->env); //execute
		if (result != 0)
		{
			exec_free(&exec);
			if (result == 1)
				printf("check_cmd Malloc Error\n");
			return (1);
		}
		printf("COMMAND:\n");
		print_tab(exec.command);
	}
	//remonter->parent et if (is_operator)
	//recursif(option 2);
	exec_free(&exec);
	printAST(p->ast, 0);
	return (0);
}

void	redirect_pipe(t_ast *current, t_exec *exec)
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

int		assign_redirect(t_ast *current, t_exec *exec)
{
	while (current && (exec->redirectin == 0 || exec->redirectout == 0))
	{
		if (is_operator(current->type, 2) == 1) //En remontant Si je croise un pipe, pipe 1 = gauche/ pipe 2 = millieu / pipe 3 = droite
		{
			if (current->type == TOKEN_PIPE)
				redirect_pipe(current, exec);
			else
			{
				if (exec->in != NULL)
					exec->redirectin = 1;
				if (exec->out != NULL)
					exec->redirectout = 1;
			}
		}
		if ((current->type == WORD_FILEIN || current->type == WORD_LIMITER) && exec->redirectin == 0)
			exec->in = current;
		else if ((current->type == WORD_FILEOUT || current->type == WORD_FILEOUT_APPEND) && exec->redirectout == 0)
			exec->out = current;
		if (!current->parent)
			break;
		current = current->parent;
	}
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