/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 09:25:36 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
ECHO builtin command main handler.
Right now, it manages -n flag and prints tokens.
Tokens that need expansion are received already expanded.
TODO: handle cases with multiple -n
TODO: correctly echo arguments like (echo '"hola'$USER'$USER"') (without spaces)
TODO: should echo this >>> "holanponchon$USER"
*/
void	ms_echo(t_ms *ms)
{
	int		n_flag;
	t_list	*current;

	n_flag = 0;
	current = ms->exec_tokens->next;
	if (current && !ft_strncmp(current->content, "-n", 2))
	{
		n_flag = 1;
		current = current->next;
	}
	if (current && current->content
		&& !ft_strncmp(current->content, "?", 1) && !current->next)
	{
		ft_printf("%d\n", ms_get_set(0, 0));
		return ;
	}
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
