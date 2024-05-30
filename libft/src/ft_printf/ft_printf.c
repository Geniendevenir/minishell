/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adebert <adebert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 16:21:49 by adebert           #+#    #+#             */
/*   Updated: 2023/12/08 18:01:23 by adebert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	check_format(char c, va_list ap)
{
	int	count;
	int	sign;

	sign = 0;
	count = 0;
	if (c == 's')
		count = ft_putstr(va_arg(ap, char *));
	else if (c == 'd' || c == 'i')
		count = ft_putnbr_decimal(va_arg(ap, int), sign);
	else if (c == 'u')
		count = ft_putnbr_unsigned(va_arg(ap, unsigned int), sign);
	else if (c == 'x')
		count = ft_putnbr_hexa(va_arg(ap, int), 0);
	else if (c == 'X')
		count = ft_putnbr_hexa(va_arg(ap, int), 1);
	else if (c == 'p')
		count = ft_check_ptr(va_arg(ap, unsigned long));
	else if (c == 'c')
		count = ft_putchar(va_arg(ap, int));
	else if (c == '%')
		count = write (1, "%", 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;
	int		count;

	va_start(ap, format);
	if (!format)
		return (-1);
	count = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			count += check_format(format[i + 1], ap);
			i++;
		}
		else
		{
			write (1, &format[i], 1);
			count++;
		}
		i++;
	}
	va_end(ap);
	return (count);
}
