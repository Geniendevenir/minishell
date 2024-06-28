/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:53:13 by allan             #+#    #+#             */
/*   Updated: 2024/06/28 14:08:36 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
	Check le nombre d'option
	Check si numeric / signe
	Check si compris entre 0 et 255
		if > 255 && < long long int max: Exit satus % 256
		if > long long int max:
			exit
			bash: exit: -9223372036854775808043453453: numeric argument required + $? = 2
		if long long int min > && < 0: Exit status % 256
		if < long long int min
			exit
			bash: exit: 92233720368547758070: numeric argument required + $? = 2
	Check si le shell est interactif
	Check si dans un pipe
	Check si dans subshell: si oui print exit
*/