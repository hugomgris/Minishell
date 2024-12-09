/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:56:13 by nponchon          #+#    #+#             */
/*   Updated: 2024/12/09 18:04:22 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_key_checker(char *key, const char *var)
{
	int	i;

	i = -1;
	while (key[++i])
	{
		if (key[i] != var[i])
			return (FALSE);
		if (key[i + 1] == '\0' && var[i + 1] == '=')
			return (TRUE);
	}
	return (FALSE);
}
