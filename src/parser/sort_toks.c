/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_toks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:46:25 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/18 18:20:57 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_sort_toks(t_token *toks)
{
	t_token	*current;

	current = toks;
	while (current)
	{
		if (current->type)
		{
			return ;
		}
		current = current->next;
	}
}
