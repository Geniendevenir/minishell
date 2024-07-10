/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:13 by allan             #+#    #+#             */
/*   Updated: 2024/07/01 17:12:56 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
	REGARDE LE PREMIER ARGUMENT
	Check si numeric / signe Sinon:
		exit
		bash: exit: 12e: numeric argument required + $? = 2
	Check si compris entre 0 et 255
		if > 255 && < long long int max: Exit satus % 256
		if > long long int max:
			exit
			bash: exit: -9223372036854775808043453453: numeric argument required + $? = 2
		if long long int min > && < 0: Exit status % 256
		if < long long int min
			exit
			bash: exit: 92233720368547758070: numeric argument required + $? = 2
	
	SI VALIDE:
	Check le nombre d'option Sinon:
		exit
		bash: exit: too many arguments + $? = 1
	Check si le shell est interactif !!!
	Check si dans un pipe !!! CHECK HISTOIRE D'OFFSET
	Check si dans subshell: si oui print exit
	printf("test1\n");
*/

int	ft_exit(char **commande, bool child)
{
	long long int	exit_status;
	
	if (!commande || !*commande)
	{
		check_is_child(commande[0], child, 0);
		return (0);
	}
	if (check_is_num(commande[0]) == 1)
	{
		check_is_child(commande[0], child, 1);
		//$? == 2
		return (2); //Est ce que ce return equivaut a la valeur de $?
	}
	exit_status = ft_atoi(commande[0]);
	if (check_size(&exit_status) == 1)
	{
		check_is_child(commande[0], child, 1);
		return (2);
	}
	if (commande[1] != NULL)
	{
		check_is_child(NULL, child, 2);
		return (2);
	}
	return ((int) exit_status);
}

int	check_is_num(char *exit_status)
{
	int	i;

	i = 0;
	while (exit_status[i])
	{
		if (ft_isdigit(exit_status[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	check_size(long long int *exit_status)
{
	printf("check size = %lld\n", *exit_status);
	if ((*exit_status > LLONG_MAX) 
		|| (*exit_status < LLONG_MIN))
		return (1);
	if ((*exit_status > 255 && *exit_status < LLONG_MAX) 
		|| (*exit_status < 0 && *exit_status > LLONG_MIN))
	{
			
		*exit_status = *exit_status % 256;
	}
	return (0);
}

void check_is_child(char *commande, bool child, int error)
{
	if (child == 1)
		write(1, "exit\n", 5);
	if (error == 1)
	{
		write(1, "bash: exit: ", 12); //bash: exit: commande[0]: numeric argument required
		write(1, commande, ft_strlen(commande));
		write(1, ": numeric argument required\n", 28);
	}
	if (error == 2)
		write(1, "bash: exit: too many arguments\n", 31);
}
