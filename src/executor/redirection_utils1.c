/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/30 09:22:17 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_filter_args(t_ms *ms)
{
	int	count;

	count = ms_count_non_redirectors(ms->cmd_args);
	ms->filt_args = ms_allocate_filtered_args(ms, count);
	if (!ms->filt_args)
		return ;
	ms_populate_filtered_args(ms, count);
}

int	ms_count_non_redirectors(char **cmd_args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd_args[i])
	{
		if (!ms_detect_redirector(cmd_args[i]) && cmd_args[i][0] != '<')
			count++;
		i++;
	}
	return (count);
}

char	**ms_allocate_filtered_args(t_ms *ms, int count)
{
	char	**filtered;

	filtered = malloc(sizeof(char *) * (count + 1));
	if (!filtered)
		ms_error_handler(ms, "Error: Mem alloc failed", 1);
	return (filtered);
}

void	ms_populate_filtered_args(t_ms *ms, int count)
{
	int	i;
	int	f_pos;

	i = 0;
	f_pos = 0;
	while (ms->cmd_args[i] && f_pos < count)
	{
		if (ms_detect_redirector(ms->cmd_args[i]) || ms->cmd_args[i][0] == '<')
			i += 2;
		else
		{
			ms->filt_args[f_pos] = ms->cmd_args[i];
			f_pos++;
			i++;
		}
	}
	ms->filt_args[f_pos] = NULL;
}

int	ms_handle_open_error(t_ms *ms, char *filename)
{
	char	*output;

	cucufu(1);
	output = ft_strjoin(filename, ": No such file or directory");
	gc_add(output, &ms->gc);
	ms_error_handler(ms, output, 0);
	if (ms->pipe_count || !ms_is_builtin(ms->filt_args[0]))
		exit (1);
	return (-1);
}
