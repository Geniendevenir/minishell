/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebert <adebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 13:19:47 by adebert           #+#    #+#             */
/*   Updated: 2023/12/08 16:43:54 by adebert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

unsigned int	ft_putnbr_unsigned(unsigned int n, int sign)
{
	char	c;

	if (n <= 9)
	{
		c = n + '0';
		write(1, &c, 1);
	}
	if (n > 9)
	{
		ft_putnbr_decimal(n / 10, sign);
		ft_putnbr_decimal(n % 10, sign);
	}
	return (ft_len_unsigned_int(n, sign));
}

int	ft_putnbr_decimal(int n, int sign)
{
	char	c;

	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	else if (n < 0)
	{
		write(1, "-", 1);
		n *= -1;
		sign = 1;
	}
	if (n <= 9)
	{
		c = n + '0';
		write(1, &c, 1);
	}
	if (n > 9)
	{
		ft_putnbr_decimal(n / 10, sign);
		ft_putnbr_decimal(n % 10, sign);
	}
	return (ft_len_int(n, sign));
}

int	ft_putnbr_hexa(unsigned int n, int caps)
{
	if (n >= 16)
	{
		ft_putnbr_hexa(n / 16, caps);
		ft_putnbr_hexa(n % 16, caps);
	}
	else
	{
		if (n <= 9)
			ft_putchar(n + '0');
		else
		{
			if (caps == 0)
				ft_putchar(n - 10 + 'a');
			else if (caps == 1)
				ft_putchar(n - 10 + 'A');
		}
	}
	return (ft_len_hexa(n));
}

int	ft_putnbr_ptr(unsigned long n)
{
	if (n >= 16)
	{
		ft_putnbr_ptr(n / 16);
		ft_putnbr_ptr(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar(n + '0');
		else
			ft_putchar(n - 10 + 'a');
	}
	return (ft_len_hexa(n));
}

int	ft_check_ptr(unsigned long n)
{
	int		count;

	count = 0;
	if (n == 0)
		return (write(1, "(nil)", 5));
	else
	{
		count = write(1, "0x", 2);
		count += ft_putnbr_ptr(n);
	}
	return (count);
}
