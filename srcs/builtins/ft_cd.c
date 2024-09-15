/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 13:05:18 by Matprod           #+#    #+#             */
/*   Updated: 2024/09/15 09:30:38 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_in_env(t_env *env, char *key)
{
	if (!ft_is_in_env(env, key))
		return (NULL);
	while (ft_strncmp(env->key, key, ft_strlen(key)))
		env = env->next;
	return (env->value);
}

static void	update_env(t_env *env, char *old, char *new)
{
	if (ft_is_in_env(env, old))
	{
		while (ft_strcmp(env->key, old))
			env = env->next;
		free(env->value);
		env->value = new;
	}
	else
		free(new);
}

static void	update_pwd(t_env *env, char *oldpwd)
{
	char	buffer[1024];
	char	*pwd;

	if (getcwd(buffer, 1024))
	{
		pwd = ft_strdup(buffer);
		if (!pwd)
		{
			free(oldpwd);
			return ;
		}
		update_env(env, "PWD", pwd);
	}
	update_env(env, "OLDPWD", oldpwd);
}

char	*go_option(t_env *env, int flag)
{
	if (flag == 1)
	{
		if (ft_is_in_env(env, "HOME"))
			return (get_in_env(env, "HOME"));
		else
			ft_putendl_fd("cd : HOME not set", 2);
	}
	else
	{
		if (ft_is_in_env(env, "OLDPWD"))
			return (get_in_env(env, "OLDPWD"));
		else
			ft_putendl_fd("cd : OLDPWD not set", 2);
	}
	return (NULL);
}

int	ft_cd(t_env *env, char **cmd)
{
	char	*oldpwd;
	char	*pwd;

	if (array_size(cmd) > 2)
		return (error_builtins(NULL, 3), 1);
	if (ft_is_in_env(env, "PWD"))
		oldpwd = ft_strdup(get_in_env(env, "PWD"));
	else
		oldpwd = ft_strdup("error");
	if (!oldpwd)
		return (1);
	if (cmd[1] == NULL)
		pwd = ft_strdup(go_option(env, 1));
	else if (!ft_strcmp(cmd[1], "-"))
		pwd = ft_strdup(go_option(env, 2));
	else
		pwd = ft_strdup(cmd[1]);
	if (!pwd)
		return (free(oldpwd), 1);
	if (!chdir(pwd))
		update_pwd(env, oldpwd);
	else
		return (free(oldpwd), error_cd(pwd), 1);
	return (free(pwd), 0);
}
