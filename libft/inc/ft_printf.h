/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: allan <allan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:10:26 by adebert           #+#    #+#             */
/*   Updated: 2024/05/11 20:39:56 by allan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H

# define FT_PRINTF_H

# include <unistd.h>

# include <stdlib.h>

# include <stdio.h>

# include <stdarg.h>
# include "./libft.h" 

int				ft_check_ptr(unsigned long n);
int				ft_printf(const char *format, ...);
int				check_format(char c, va_list ap);
int				ft_len_int(int n, int sign);
int				ft_putstr(char *s);
int				ft_putchar(char c);
int				ft_putnbr_decimal(int n, int sign);
unsigned int	ft_putnbr_unsigned(unsigned int n, int sign);
int				ft_putnbr_hexa(unsigned int n, int caps);
int				ft_len_unsigned_int(unsigned int n, int sign);
int				ft_len_hexa(unsigned long n);
int				ft_putnbr_ptr(unsigned long n);
int				check_index(char format);

#endif
