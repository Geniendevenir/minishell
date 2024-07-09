/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcour_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:05:39 by Matprod           #+#    #+#             */
/*   Updated: 2024/07/02 12:25:27 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_limiter
{
	char				*limit;
	int					quotes;// pas sure
	struct s_limiter	*next;
}		t_limiter;

/* typedef struct s_file
{
	int				fd;
	int				type;
	char			*data;
	struct s_file	*next;
}	t_file; */


/* int exec_parent_node(t_ast *current, t_env *env)
{
	if (current->type == TOKEN_AND || current->type == TOKEN_OR)
		return (exec_operator(current, env));
	else if (is_command_or_builtin_or_abspath(current) == 1)
		return (exec_cmd(current, env));
	else if (current->type == TOKEN_PIPE)
		return (exec_pipe(current, env));
	else if (is_redirect_folder(current) == 1)
		return (exec_redirect(current));
	return (1);
} */

void traverse_ast(t_ast *root, t_env *env)
{
	t_ast *current;
	t_ast *last_visited;
	/* t_env *env2;

	env2 = env;
	free_env(env2); */

	current = root;
	last_visited = NULL;
	while (current)
	{
		while (current->left)
		{
			current = current->left;
			if (current->left == NULL)
				printf("Visited left node  with value: %s\n", current->value);
		}
		while (current->parent && (current->right == NULL || current->right == last_visited))
		{
			current = current->parent;
			//exec_parent_node(current, env);
			printf("Visited parent node with value: %s\n", current->value);
			//last_visited = current;
		}
		if (current->right && current->right != last_visited)
		{
			current = current->right;
			printf("Visited right node with value: %s\n", current->value);
		}
		else
			break; // Si on ne peut plus aller à droite, on a fini le parcours
		
	}
}
/*

	typedef struct s_ast
	{
		int exit_state;
		enum s_type type;
		char		*value;
		struct s_ast *left;
		struct s_ast *right;
		struct s_ast *parent;
	}				t_ast;

	typedef struct s_file
	{
		bool			available;
		int				fd;
		enum s_type		type;
		char			*data;
		struct s_file	*next; ??
	}					t_file;

	typedef struct s_exec
	{
		int subshell;
		t_file *in;
		t_file *out;
		int	pipe;
		char **command;
	}				t_exec

							EXECUTION
			I - Descendre tout a gauche et recup les infos importantes
				A - Check si option parenthese > int subshell
					Si oui fork
				B - Check si current == REDIRECT IN ou OUT ou Pipe
					si oui le save et mettre available == 1 ou pipe++;
				C - 
			II - Check si le node actuel est une commande sinon remonter jusqu'a ce que ca soit une commande
				Quand on trouve une commande on l'envoie dans une fonction qui stock la commande dans un double tableau et redescend pour stocker ses options;
			III - Check si la commande est un builtin ou un absolute path
				Si oui pas de fork
				Sinon fork
			IV - Executer la commande
				-> Rediriger les input et ouput si besoin 
*/

//FORMAT AST
/* 


								
								echo test > input.txt && < input.txt cat1 | cat2 | cat3 > output.txt
		    		VERSION 1:																						VERSION 2
	                
					    &&																								&&
	            input                    	|															input							|
	        echo                |                	output											echo						|
	    test            input        cat		cat												test						input
	                cat																									cat1
																															cat2
																																output.txt
																															cat3












									MULTIPLE REDIRECT
	echo hello world > input.txt && < input.txt (grep world | cat > ouput.txt) > test.txt				-> Toujours prendre redirection la plus a droite de l'element a rediriger
 	
	< input.txt cat | cat > output.txt && (cd test || (ls -l | > output2.txt cat))

	< input.txt cat > output.txt == >output.txt cat > input.txt ORDRE N'A PAS D'IMPORTANCE

									INCORRECT OPTIONS
	cat -f && echo hello																				-> Rien ne s'execute
		cat: invalid option -- 'f'
		Try 'cat --help' for more information.
		
	ls -e || echo test																					-> Execute le ||
		ls: invalid option -- 'e'
		Try 'ls --help' for more information.
		test

										&& et ||
	ls -e || echo test && echo lol
		ls: invalid option -- 'e'
		Try 'ls --help' for more information.
		test
		lol
	
	ls -e || cat -f  && echo lol
		ls: invalid option -- 'e'
		Try 'ls --help' for more information.
		cat: invalid option -- 'f'
		Try 'cat --help' for more information.

									BUITLINS

									MULTIPLE ||
		echo test > input.txt && < input.txt cat | cat | cat > output.txt
								&&
						input					|
					echo				|				output
				test			input		cat
							cat

*/