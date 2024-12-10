/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/09 14:55:33 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_executor(t_ms *ms)
{
	char	*trimmed_input;

	trimmed_input = ft_strtrim(ms->input, " ");
	gc_add(trimmed_input, &ms->gc);
	if (!ft_strncmp(trimmed_input, "cd", 2))
	{
		if (ft_strlen(trimmed_input) == 2)
			ms_cd(ms, NULL);
		else if (trimmed_input[2] == ' ')
			ms_cd(ms, trimmed_input + 3);
		else
			ms_error_handler(ms, "cd: invalid input", 0);
	}
}
