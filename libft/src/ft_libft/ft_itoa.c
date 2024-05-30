/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 12:07:06 by adebert           #+#    #+#             */
/*   Updated: 2023/11/29 18:21:52 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n);

static int	ft_len_int(long n)
{
	int	len_int;

	len_int = 0;
	if (n == 0)
		len_int = 1;
	if (n < 0)
	{
		n *= (-1);
		len_int++;
	}
	while (n > 0)
	{
		n /= 10;
		len_int++;
	}
	return (len_int);
}

char	*ft_itoa(int n)
{
	int		len_int;
	int		neg;
	char	*arr;
	long	nb;

	nb = (long) n;
	len_int = ft_len_int(nb);
	arr = malloc((len_int + 1) * sizeof(char));
	if (!arr)
		return (NULL);
	arr[len_int] = '\0';
	neg = 0;
	if (nb < 0)
	{
		arr[0] = '-';
		nb *= -1;
		neg = 1;
	}
	while (len_int > neg)
	{
		arr[len_int -1] = (nb % 10 + 48);
		nb /= 10;
		len_int--;
	}
	return (arr);
}
