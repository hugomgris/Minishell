/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/10 15:56:09 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Filters the arguments from the command line, excluding redirection operators
	and special characters.
Allocates memory for the filtered arguments and populates it
	with valid command arguments.
If memory allocation fails, returns to avoid minishell crash/hang.
*/
void	ms_filter_args(t_ms *ms)
{
	int	count;

	count = ms_count_non_redirectors(ms->cmd_args);
	ms->filt_args = ms_allocate_filtered_args(ms, count);
	if (!ms->filt_args)
		return ;
	ms_populate_filtered_args(ms, count);
}

/*
Counts the number of arguments in the command line that are
	not redirection operators or special characters.
This is used to determine how many valid command arguments are present.
*/
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

/*
Allocates memory for an array of filtered arguments based on the given count.
Returns the allocated memory or handles memory allocation failure
	by calling the error handler.
*/
char	**ms_allocate_filtered_args(t_ms *ms, int count)
{
	char	**filtered;

	if (count)
	{
		filtered = malloc(sizeof(char *) * (count + 1));
		if (!filtered)
			ms_error_handler(ms, "Error: Mem alloc failed", 1);
		return (filtered);
	}
	return (NULL);
}

/*
Populates the filtered arguments array by iterating
	through the original command arguments.
Redirection operators and special characters are skipped,
	while valid command arguments are added to the filtered list.
The final position is marked with a NULL terminator.
*/
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
			ms->filt_args[f_pos] = ft_strdup(ms->cmd_args[i]);
			gc_add(ms->filt_args[f_pos], &ms->gc);
			f_pos++;
			i++;
		}
	}
	ms->filt_args[f_pos] = NULL;
}

/*
Handles errors when attempting to open a file by generating an error message
	indicating the specific file does not exist.
If the error occurs in a pipe or non-builtin command context, the program exits.
Returns -1 after handling the error.
TODO: Again, check return values for consistency (0-1 vs 0-(-1))
*/
int	ms_handle_open_error(t_ms *ms, char *filename)
{
	char	*output;
	int		err;

	err = errno;
	if (err == ENOENT)
		output = ft_strjoin(filename, ": No such file or directory");
	else if (err == EACCES)
		output = ft_strjoin(filename, ": Permission denied");
	else if (err == EISDIR)
		output = ft_strjoin(filename, ": Is a directory");
	else
		output = ft_strjoin(filename, ": Unknown error");
	gc_add(output, &ms->gc);
	ms_error_handler(ms, output, 0);
	if (ms->pipe_count || !ms_is_builtin(ms->filt_args[0]))
		exit (1);
	return (-1);
}
