/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 15:49:33 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
ECHO builtin command main handler.
Right now, it manages -n flag and prints tokens.
Tokens that need expansion are received already expanded.
TODO: handle cases with multiple -n
*/
int	ms_echo(char **cmd_args)
{
	int		n_flag;
	int		i;

	n_flag = 0;
	i = 1;
	if (cmd_args[i] && !ft_strncmp(cmd_args[i], "-n", 2))
	{
		n_flag = 1;
		i++;
	}
	if (cmd_args[i] && !ft_strncmp(cmd_args[i], "?", 1) && !cmd_args[i + 1])
	{
		ft_printf("%d\n", ms_get_set(0, 0));
		return (0);
	}
	while (cmd_args[i])
	{
		ft_putstr_fd(cmd_args[i], 1);
		if (cmd_args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
