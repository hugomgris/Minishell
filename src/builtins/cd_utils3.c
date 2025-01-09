/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:37:10 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/08 20:20:16 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Checks if path is symlink, returning 0 or confirmation.
*/
int	ms_is_symlink(char *path)
{
	struct stat	path_stat;

	if (lstat(path, &path_stat) == -1)
		return (0);
	return (S_ISLNK(path_stat.st_mode));
}

/*
Helper function that makes minishell able to handle symlinks.
Using the ms_is_symlink function, returns the resolve symlink path.
	(This means that if symlink, the resolved path has symlink ref;
	if not, the resolved path has objective dir ref).
*/
char	*ms_resolve_symlink(t_ms *ms, char *symlink)
{
	char	*cwd;
	char	*resolved_path;
	char	*tmp;

	if (!symlink)
		return (NULL);
	if (symlink[0] == '/')
		return (strdup(symlink));
	cwd = ms_getcwd_or_error(ms);
	if (!cwd)
		return (NULL);
	gc_add(cwd, &ms->gc);
	resolved_path = ft_strjoin(cwd, "/");
	if (!resolved_path)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1), NULL);
	gc_add(resolved_path, &ms->gc);
	tmp = ft_strjoin(resolved_path, symlink);
	resolved_path = tmp;
	if (!resolved_path)
		return (ms_error_handler(ms, "Error: Mem alloc failed", 1), NULL);
	if (ms_is_symlink(resolved_path))
		return (resolved_path);
	return (resolved_path);
}

/*
Helper function that returns parent path from the CWD.
*/
char	*ms_get_parent_path(t_ms *ms, char *cwd)
{
	cwd = ms_expand_tilde(ms, cwd);
	gc_add(cwd, &ms->gc);
	cwd = ft_strjoin(cwd, "/..");
	gc_add(cwd, &ms->gc);
	if (ft_strchr_n(ms_normalize_path(ms, cwd), '/', 2)
		> ft_strchr(ms_normalize_path(ms, cwd), '/'))
		cwd = ft_strchr_n(ms_normalize_path(ms, cwd), '/', 2);
	else
		cwd = ft_strchr(ms_normalize_path(ms, cwd), '/');
	if (cwd == NULL)
	{
		cwd = ft_strdup("/");
		gc_add(cwd, &ms->gc);
	}
	return (cwd);
}

/*
Helper function to check if attempted cd path is a directory.
It is used to handle specific cd error outputs.
*/
int	ms_cd_isdirectory(t_ms *ms, char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		if (errno == ENOENT)
			ms_error_handler(ms, "cd: No such file or directory", 0);
		else if (errno == EACCES)
			ms_error_handler(ms, "cd: Permission denied", 0);
		else
			ms_error_handler(ms, "cd: An error ocurred", 0);
		return (-1);
	}
	if (S_ISDIR(path_stat.st_mode))
		return (1);
	else if (S_ISREG(path_stat.st_mode))
		return (0);
	else
		return (-1);
}

/*
Flow control function to check arguments sent to cd builtin cmd.
It is the first check that a cd call needs to pass.
*/
char	*ms_cd_initial_path(t_ms *ms)
{
	char	*path;
	int		i;

	if (ft_array_count(ms->cmd_args) < 2)
		return (NULL);
	i = 2;
	path = ft_strdup(ms->cmd_args[1]);
	gc_add(path, &ms->gc);
	while (ms->cmd_args[i])
	{
		path = ft_strjoin3(path, " ", ms->cmd_args[i]);
		gc_add(path, &ms->gc);
		i++;
	}
	gc_add(path, &ms->gc);
	if (!path)
		return (NULL);
	return (path);
}
