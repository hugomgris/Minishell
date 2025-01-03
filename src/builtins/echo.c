/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/02 19:15:50 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to handle -n flag for echo builtin command.
Handles -n flag in any combination (-n, -n -n -n, -nnnn, -n-n-n).
*/
int	ms_echo_check_n_flag(char **args, int *i)
{
	int	n_flag;

	n_flag = 0;
	while (args[*i] && !ft_strncmp(args[*i], "-n", 2)
		&& (!args[*i][2] || args[*i][2] == 'n'))
	{
		n_flag = 1;
		(*i)++;
	}
	return (n_flag);
}

/*
Helper function to print echo builtin arguments.
*/
void	ms_echo_print_args(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

/*
ECHO builtin command main handler.
Handles -n flag via helper function.
Tokens that need expansion are received already expanded.
*/
int	ms_echo(char **cmd_args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = ms_echo_check_n_flag(cmd_args, &i);
	ms_echo_print_args(cmd_args, i);
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
