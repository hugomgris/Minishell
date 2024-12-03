/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/03 20:19:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_cd_absolute(t_ms *ms, char *path)
{
	char	*cwd;
	char	*new_cwd;

	cwd = ms_getcwd_or_error(ms);
	gc_add(cwd, &ms->gc);
	if (ms_change_directory(ms, path) == -1)
		return ;
	gc_add(cwd, &ms->gc);
	ms_set_env_variable(ms, "OLDPWD", cwd);
	new_cwd = ms_getcwd_or_error(ms);
	ms_set_env_variable(ms, "PWD", new_cwd);
}

void	ms_cd_home(t_ms *ms)
{
	char	*home_dir;
	char	*cwd;

	home_dir = ms_get_env_variable(ms, "HOME=");
	if (!home_dir)
	{
		ms_error_handler(ms, "cd: Home not set", 0);
		return ;
	}
	cwd = ms_getcwd_or_error(ms);
	gc_add(cwd, &ms->gc);
	if (ms_change_directory(ms, home_dir) == -1)
		return ;
	ms_set_env_variable(ms, "OLDPWD", cwd);
	cwd = ms_getcwd_or_error(ms);
	gc_add(cwd, &ms->gc);
	ms_set_env_variable(ms, "PWD", cwd);
}

void	ms_cd_back(t_ms *ms)
{
	char	*old_pwd;
	char	*cwd;
	char	*new_pwd;

	old_pwd = ms_get_env_variable(ms, "OLDPWD=");
	if (!old_pwd)
	{
		ms_error_handler(ms, "cd: OLDPWD not set", 0);
		return ;
	}
	cwd = ms_getcwd_or_error(ms);
	if (ms_change_directory(ms, old_pwd) == -1)
	{
		free(cwd);
		return ;
	}
	gc_add(cwd, &ms->gc);
	ms_set_env_variable(ms, "OLDPWD", cwd);
	new_pwd = ms_getcwd_or_error(ms);
	gc_add(new_pwd, &ms->gc);
	ms_set_env_variable(ms, "PWD", new_pwd);
}

void	ms_cd_relative(t_ms *ms, char *path)
{
	char	cwd[PATH_MAX];
	char	*new_path;

	path = ft_strtrim(path, "/");
	gc_add(path, &ms->gc);
	if (getcwd(cwd, PATH_MAX) == NULL)
		ms_error_handler(ms, "Error: couldn't find path", 0);
	else
	{
		if (ms_join_paths(cwd, path, &new_path) == -1)
			ms_error_handler(ms, "Error: mem alloc failed", 1);
		if (ms_change_directory(ms, new_path) == -1)
			return ;
		ms_set_env_variable(ms, "OLDPWD", cwd);
		ms_set_env_variable(ms, "PWD", new_path);
		gc_add(new_path, &ms->gc);
	}
}

void	ms_cd(t_ms *ms, char *path)
{
	if (!path || path[0] == '~' || path[0] == '\0')
		ms_cd_home(ms);
	else if (path[0] == '/' && path[1])
		ms_cd_absolute(ms, path);
	else if (path[0] && !path[1])
		ms_cd_root(ms); //! this is not yet implemented!! AND ITS NOT NEEDED BY I DON'T CARE YOU CAN'T STOP ME HAHAHAHA
	else if (path[0] == '-')
		ms_cd_back(ms);
	else
		ms_cd_relative(ms, path);
}
