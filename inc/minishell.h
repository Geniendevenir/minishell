/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:24 by Matprod           #+#    #+#             */
/*   Updated: 2024/06/03 16:19:30 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <limits.h>
# include <string.h>
# include <signal.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <errno.h>
# include <poll.h>
# include <sys/msg.h>
# include <../libft/inc/libft.h>
# include <../libft/inc/ft_printf.h>
# include <../libft/inc/get_next_line.h>


typedef struct s_env
{
	char			*key;
	char			*value;
	int				code;
	struct s_env	*next;
}	t_env;

typedef struct s_sig
{
	int		sig_quit;
	int		sig_int;
	int		p_status;
	int		cmd_stat;
}	t_sig;

typedef struct s_all
{
	t_env	*env;
	char	*line;
	t_sig	*sig;
}	t_all;


extern t_sig	g_sig;

/*					SIGNALS					*/

int	event(void);
int	create_signal(void);
/*					CHECK					*/

int	check_cmd_exist(char *word, t_env *env);

/*					 ENV					*/

t_env	*env_to_struct(char **env);

/*					 INIT					*/
t_all	*init_all(char **env);
t_sig	*init_signal(int nb);

/*					 FREE					*/

void	free_env(t_env *envp);
void	free_all(t_all *p);
void	free_array(char **array);

/*					 UTILS					*/


char	*ft_strndup(char *str, int n);
int		ft_strcmp(char *s1, char *s2);
void 	print_env(t_env *env);

#endif