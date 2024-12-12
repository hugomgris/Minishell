/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 12:21:40 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
ECHO builtin command main handler.
Right now, it manages -n flag and prints tokens.
Tokens that need expansion are received already expanded.
TODO: handle cases with multiple -n
TODO: correctly echo arguments like (echo '"hola'$USER'$USER"') (without spaces)
*/
void	ms_echo(t_list *tokens)
{
	int		n_flag;
	t_list	*current;

	n_flag = 0;
	current = tokens->next;
	if (tokens->next && !ft_strncmp(tokens->next->content, "-n", 2))
	{
		n_flag = 1;
		current = current->next;
	}
	if (current->content)
	{
		while (current)
		{
			ft_putstr_fd(current->content, 1);
			if (current->next)
				ft_putchar_fd(' ', 1);
			current = current->next;
		}
		if (!n_flag)
			ft_putchar_fd('\n', 1);
	}
}
