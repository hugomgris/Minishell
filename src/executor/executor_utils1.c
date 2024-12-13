/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/13 09:55:51 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function that creates the argument array to be sent to a cmd exec.
*/
char	**ms_make_argv(t_ms *ms, t_list *tokens)
{
	int		count;
	char	**argv;
	int		i ;

	count = ft_lstsize(tokens);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
	{
		ms_error_handler(ms, "Error: Memory allocation failed", 1);
		return (NULL);
	}
	i = 0;
	while (tokens)
	{
		argv[i++] = ft_strdup(tokens->content);
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

char	*ms_get_env_path_or_def(t_ms *ms)
{
	char	*path;

	path = ms_get_env_variable(ms, "PATH");
	if (!path)
	{
		path = "/bin:/usr/local/sbin:/usr/local";
		path = ft_strjoin(path, "/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		gc_add(path, &ms->gc);
	}
	return (path);
}

/*
Flow control function that searches for commands in the system.
If command exists, returns its path so it can be executed.
*/
char	*ms_get_command_path(t_ms *ms, char *cmd)
{
	char	*prepath;
	char	**paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	prepath = ms_get_env_path_or_def(ms);
	paths = ft_split(prepath, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		gc_add(full_path, &ms->gc);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

/*
Helper function to check if an input command exists. 
It is used to end execution if an invalid command is input.
*/
char	*validate_command(t_ms *ms)
{
	char	*path;

	path = ms_get_command_path(ms, ms->tokens->content);
	if (!path)
		ms_error_handler(ms, "Command not found", 0);
	return (path);
}

/*
Re-creates an array of env variables from ms_env linked list initial copy.
It is needed to send to child processes as their env array.
*/
char	**ms_env_to_array(t_list *ms_env)
{
	int		count;
	int		i;
	char	**arr;
	t_list	*current;

	count = ft_lstsize(ms_env);
	arr = malloc(sizeof(char *) * (count + 1));
	i = 0;
	current = ms_env;
	if (!arr)
		return (NULL);
	while (current)
	{
		arr[i] = (char *)current->content;
		current = current->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
