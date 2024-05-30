/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Matprod <matprod42@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:15:53 by Matprod           #+#    #+#             */
/*   Updated: 2024/05/30 11:39:49 by Matprod          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main() {
    char *input;

    while (1) {
        input = readline("\033[1;032mMinishell> \033[m");
        if (input && *input) {
            add_history(input);
        }
        free(input);
    }
    return 0;
}