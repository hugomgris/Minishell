/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 14:07:59 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_cd_symlink(t_ms *ms, char *path)
{
	char	*resolved_path;

	if (ms_is_symlink(path))
	{
		resolved_path = ms_resolve_symlink(ms, path);
		if (resolved_path)
		{
			if (ms_change_directory(ms, resolved_path) == -1)
				return ;
			gc_add(resolved_path, &ms->gc);
		}
		else
			ms_error_handler(ms, "cd: Failed to resolve symlink", 0);
	}
	else
		if (ms_change_directory(ms, path) == -1)
			return ;
}

void	ms_cd_home(t_ms *ms)
{
	if (ms->home == NULL)
	{
		ms_error_handler(ms, "cd: HOME not set", 0);
		return ;
	}
	if (ms_change_directory(ms, ms->home) == -1)
		return ;
}

void	ms_cd_back(t_ms *ms)
{
	char	*aux;

	if (!ms_get_env_variable(ms, "OLDPWD="))
	{
		ms_error_handler(ms, "cd: OLDPWD not set", 0);
		return ;
	}
	aux = ft_strdup(ms_get_env_variable(ms, "OLDPWD="));
	gc_add(aux, &ms->gc);
	if (ms_change_directory(ms, aux) == -1)
		return ;
	ft_printf("%s\n", aux);
}

void	ms_cd_parent(t_ms *ms)
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
		return ;
	}
	if ((try && chdir(cwd) == -1))
	{
		try--;
		ms_error_handler(ms, error, 0);
		return ;
	}
	try++;
	return (ms_cd_ascend(ms));
}

void	ms_cd(t_ms *ms, char *path)
{
	if (!path || path[0] == '\0' || (path[0] == '~' && !path[1]))
		return (ms_cd_home(ms));
	else if (!ft_strncmp(path, "...", 3))
	{
		ms_error_handler(ms, "cd: No such file or directory", 0);
		return ;
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
	ms_cd_symlink(ms, path);
}
