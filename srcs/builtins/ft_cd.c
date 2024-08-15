/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:28 by allan             #+#    #+#             */
/*   Updated: 2024/07/22 18:00:35 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
A path is absolute if the first character is a /. Otherwise, it is a relative path.
Absolute paths will take you to any directory, from any directory. Relative paths will only take you to directories below your current one.

ABSOLUTE PATH: cd /
	Used to look down from the root 
	ex:	Dir1 -> If Dir1 is the root "/" will start from here and look down for any matching directory
		Dir2
		Dir3 -> Current Directory

Relative PATH:
	path that doesnt start with /, int that case cd will only look down from the Current directory NOT UP

Special:	
			cd . = 
			cd .. = Move up one directory. cd ../.. = Moove up tw directories
			cd / = Takes you to the root directory
			cd /home = takes you to home directory 
			cd /root = bash: cd: /root: Permission denied : Only accessible when logged in as root user, Should we tak care of it ???
*/

//CHECK SI BESOIN D'implementer OLDPWD=/home/allan/project/minishell

int	ft_cd(char *path)
{
	DIR				*d;
	char			*new_dir;
	int				error;
	
	error = 1;
	if (!path)
		return (1); //no path
	if (*path == '/')
		error = chdir(path); //OK
	else
	{
		d = opendir(".");
		if (!d)
		{
			error_executer(NULL, 7);
			return (1);
		}
		new_dir = relative_path(d, path, &error);
		closedir(d);
		if (!new_dir)
			return (1);
		error = chdir(new_dir);
		//free(new_dir);
	}
	return(error);
}

char	*relative_path(DIR	*d, char *path, int *error)
{
	struct dirent	*try_dir;
	struct stat 	file_stat;
	char			cur_dir[1024];
	char			*new_dir;
	
	if (getcwd(cur_dir, 1024) == NULL) //comment join avec 
		return (NULL); //add error
	while (1)
	{
		try_dir = readdir(d);
		if (!try_dir)
			break ; 
		else if (ft_strcmp(try_dir->d_name, path) == 0)
		{
			if (lstat(try_dir->d_name, &file_stat) != 0)
			{
				error_builtins(NULL, 1);
				return (NULL);
			}
			if (S_ISDIR(file_stat.st_mode) != 0)
			{
				error_builtins(try_dir->d_name, 1);
				return (NULL);
			}
			new_dir = cd_match(cur_dir, try_dir->d_name, error);
			if (*error != 0)
			{
				if (*error == 3)
					free(new_dir);
				return (NULL);
			}
			break ;
		}
	}
	*error = 0;
	return (new_dir);
}

char	*cd_match(char *cur_dir, char *try_dir, int *error)
{
	char	*full_path;
	char	*new_dir;
	
	full_path = ft_strjoin(cur_dir, "/");
	if (!full_path)
	{
		*error = 2;
		return (NULL);
	}
	new_dir = ft_strjoin(full_path, try_dir);
	free(full_path);
	if (!new_dir)
	{
		*error = 3;
		return (NULL);
	}
	*error = 0;
	return (new_dir);
}
	
	//ABSOLUT PATH: just use chdir
	//RELATIVE PATH: opendir(.) -> Strcmp avec tous les fichiers -> Strdup -> closedir -> get cdw -> strjoin -> chdir -> free + return
	
	//get cwd
	//stat
	//chdir