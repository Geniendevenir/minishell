/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_len_int.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebert <adebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 13:19:47 by adebert           #+#    #+#             */
/*   Updated: 2023/12/08 16:42:49 by adebert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_len_int(int n, int sign)
{
	int	len_int;

	len_int = 0;
	if (n == 0)
		len_int = 1;
	if (sign == 1)
		len_int++;
	while (n > 0)
	{
		n /= 10;
		len_int++;
	}
	return (len_int);
}

int	ft_len_unsigned_int(unsigned int n, int sign)
{
	int	len_int;

	len_int = 0;
	if (n == 0)
		len_int = 1;
	if (sign == 1)
		len_int++;
	while (n > 0)
	{
		n /= 10;
		len_int++;
	}
	return (len_int);
}

int	ft_len_hexa(unsigned long n)
{
	int	len_int;

	len_int = 0;
	if (n == 0)
		len_int = 1;
	while (n > 0)
	{
		n /= 16;
		len_int++;
	}
	return (len_int);
}
