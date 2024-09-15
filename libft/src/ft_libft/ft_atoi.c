/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 01:21:12 by allan             #+#    #+#             */
/*   Updated: 2024/04/15 20:26:26 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *nptr);

long	ft_atoi(const char *nptr)
{
	int		sign;
	long	nbr;

	nbr = 0;
	sign = 1;
	if (*nptr == '\0')
		return (0);
	while (*nptr && (*nptr == ' ' || *nptr == '\t'
			|| *nptr == '\n' || *nptr == '\r'
			|| *nptr == '\v' || *nptr == '\f'))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = sign * -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		nbr = (nbr * 10) + (*nptr - '0');
		nptr++;
	}
	return (nbr * sign);
}
