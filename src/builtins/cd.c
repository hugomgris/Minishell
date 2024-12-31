/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/31 10:35:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Makes cd able to handle symlinks.
All cding with normalized paths go through here to check if symlink.
If argument is a non-working symlink, outputs error.
Else, calls change directory function.
*/
int	ms_cd_symlink(t_ms *ms, char *path)
{
	char	*resolved_path;

	if (ms_is_symlink(path))
	{
		resolved_path = ms_resolve_symlink(ms, path);
		if (resolved_path)
		{
			if (ms_change_directory(ms, resolved_path) == -1)
				return (1);
			gc_add(resolved_path, &ms->gc);
		}
		else
		{
			ms_error_handler(ms, "cd: Failed to resolve symlink", 0);
			return (1);
		}
	}
	else
		if (ms_change_directory(ms, path) == -1)
			return (1);
	return (0);
}

/*
Handles cding to home dir (~ or !path).
If there is no HOME ref in ms struct, outputs error.
*/
int	ms_cd_home(t_ms *ms)
{
	if (ms->home == NULL)
	{
		ms_error_handler(ms, "cd: HOME not set", 0);
		return (1);
	}
	if (ms_change_directory(ms, ms->home) == -1)
		return (1);
	return (0);
}

/*
Handles cding into last visited dir.
Last visited dir is taken from ms_env (OLDPWD).
If OLDPWD is unset, outputs error.
*/
int	ms_cd_back(t_ms *ms)
{
	char	*aux;

	if (!ms_get_env_variable(ms, "OLDPWD"))
	{
		ms_error_handler(ms, "cd: OLDPWD not set", 0);
		return (1);
	}
	aux = ft_strdup(ms_get_env_variable(ms, "OLDPWD"));
	gc_add(aux, &ms->gc);
	if (ms_change_directory(ms, aux) == -1)
		return (1);
	ft_printf("%s\n", aux);
	return (0);
}

/*
Handles cding into parent directory (cd ..).
Calls function that searches for parent dir.
Safely works from a non existing dir or absence of parent dir as bash does:
	-First attempt outputs specific error.
	-Second attempt calls ascend function to search for closest available parent.
*/
int	ms_cd_parent(t_ms *ms)
{
	static int	try = 1;
	char		*cwd;
	char		*error;

	error = "cd: Cannot access parent directories: No such file or directory";
	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (access(cwd, F_OK) != -1)
	{
		gc_add(cwd, &ms->gc);
		cwd = ms_get_parent_path(ms, cwd);
		ms_change_directory(ms, cwd);
		return (0);
	}
	if ((try && chdir(cwd) == -1))
	{
		try--;
		ms_error_handler(ms, error, 0);
		return (0);
	}
	try++;
	return (ms_cd_ascend(ms));
}

/*
CD builtin command hub.
Checks argument and calls a variety of cd functions depending on the case.
Special/specific cases are handled directly. Others go through normlizing.
Right now, minishell cd can handle:
	-Relative and absolute paths.
	-cd back (../), multiple times and with path insertion
	-cd - to go to the previous visited dir
	-cd ~ to go directly to home
	-cd / to go directly to root
	-Symlinks, both to existing and non existing dirs (erroring in 2nd case)
*/
int	ms_cd(t_ms *ms)
{
	char	*path;

	path = ms_cd_initial_path(ms);
	if (!path || path[0] == '\0' || (path[0] == '~' && !path[1]))
		return (ms_cd_home(ms));
	else if (!ft_strncmp(path, "...", 3))
	{
		ms_error_handler(ms, "cd: No such file or directory", 0);
		return (1);
	}
	else if (path[0] == '-' && !path[1])
		return (ms_cd_back(ms));
	else if (path[0] == '/' && !path[1])
		return (ms_cd_root(ms, path));
	else if ((path[0] == '.' && path[1] == '.') && !path[2])
		return (ms_cd_parent(ms));
	if (path[0] == '~')
		path = ms_expand_tilde(ms, path);
	path = ms_normalize_path(ms, path);
	gc_add(path, &ms->gc);
	return (ms_cd_symlink(ms, path));
}
