/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:48:05 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/26 21:30:39 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../libft/inc/libft.h"

/*check_file : 0 = file don't exist | 1 = file exist*/
int	check_file(char *word)
{
	struct stat	st;

	if (stat(word, &st) == -1)
		return (0);
	else
		return (1);
}

/*check_builtin : if word is a builtin return 1, else 0*/
int	check_builtin(char *word)
{
	if (ft_strcmp(word, "echo") == 0)
		return (1);
	else if (ft_strcmp(word, "cd") == 0)
		return (1);
	else if (ft_strcmp(word, "unset") == 0)
		return (1);
	else if (ft_strcmp(word, "exit") == 0)
		return (1);
	else if (ft_strcmp(word, "env") == 0)
		return (1);
	else if (ft_strcmp(word, "export") == 0)
		return (1);
	else if (ft_strcmp(word, "pwd") == 0)
		return (1);
	else if (ft_strcmp(word, "echo") == 0)
		return (1);
	else
		return (0);
}

int	check_absolute_path_cmd(char *word)
{
	if (access(word, X_OK) == 0)
		return (1);
	else
		return (0);
}

int	check_cmd_exist(char *word, t_env *env)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (env && ft_strcmp(env->key, "PATH") != 0)
		env = env->next;
	paths = ft_split(env->value, ':');
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, word);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_array(paths);
			free(path);
			return (1);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (0);
}

enum s_type	check_word(char *word, t_word *boolean, t_env *env)
{
	if (boolean->redi_in == 1)
	{
		boolean->redi_in = 0;
		if (check_file(word) == 1)
			return (WORD_FILEIN);
		else
			return (WORD_ERROR);
	}
	else if (boolean->redi_out == 1)
		return (check_word_part_rediout(boolean));
	else if (boolean->append == 1)
		return (check_word_part_append(boolean));
	else if (boolean->here_doc == 1)
	{
		boolean->here_doc = 0;
		boolean->cmd = 0;
		return (WORD_LIMITER);
	}
	else if (boolean->cmd == 1 && boolean->operator == 0)
		return (WORD_OPTION);
	else if ((boolean->redi_in == 0) && (boolean->redi_out == 0)
		&& (boolean->cmd == 0))
		return (check_word_part_cmd(word, boolean, env));
	return (WORD_WTF);
}

/*  int main()
{
    char *path = "example.txt";
	char *word = "echo";
	char *abs_path = "/bin/grep";
	char *cmd = "cat";

	
	if (check_cmd_exist(cmd))
	{
        printf("La commande %s est exécutable.\n", cmd);
    } else {
        printf("La commande %s n'est pas exécutable.\n", cmd);
    }
     if (check_absolute_path_cmd(abs_path))
	{
        printf("La commande %s est exécutable.\n", abs_path);
    } else {
        printf("La commande %s n'est pas exécutable.\n", abs_path);
    } 

     if (check_file(path))
	{
        printf("Le fichier %s existe.\n", path);
    } else
	{
        printf("Le fichier %s n'existe pas.\n", path);
    }

	if (check_builtin(word))
	{
        printf("Le commande %s existe.\n", word);
    } else
	{
        printf("La commande %s n'existe pas.\n", word);
    } 
    return 0;
} */