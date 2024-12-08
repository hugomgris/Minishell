/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:49:43 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/04 15:59:30 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_expand_tilde(t_ms *ms, char*path)
{
	char	*home;
	char	*expanded_path;

	home = ms_get_env_variable(ms, "HOME=");
	if (!home)
	{
		ms_error_handler(ms, "Minishell: cd: HOME not set", 0);
		return (NULL);
	}
	expanded_path = ft_strjoin(home, path + 1);
	if (!expanded_path)
		ms_error_handler(ms, "Error: mem alloc failed", 1);
	gc_add(expanded_path, &ms->gc);
	return (expanded_path);
	ms_error_handler(ms, "Minishell: cd: unsupported use of ~", 0);
	return (NULL);
}

int	ms_join_paths(char *cwd, char *path, char **new_path)
{
	size_t	cwd_len;
	size_t	path_len;
	size_t	total_len;

	cwd_len = ft_strlen(cwd);
	path_len = ft_strlen(path);
	total_len = cwd_len + path_len + 2;
	*new_path = malloc(total_len);
	if (!*new_path)
		return (-1);
	ft_strlcpy(*new_path, cwd, total_len);
	ft_strlcat(*new_path, "/", total_len);
	ft_strlcat(*new_path, path, total_len);
	return (0);
}

int	ms_change_directory(t_ms *ms, char *new_path)
{
	if (chdir(new_path) == -1)
	{
		gc_add(new_path, &ms->gc);
		ms_error_handler(ms, "minishell: cd: no such file or path", 0);
		return (-1);
	}
	return (0);
}

char	*ms_getcwd_or_error(t_ms *ms)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ms_error_handler(ms, "minishell: cd: couldn't get CWD", 0);
		return (NULL);
	}
	gc_add(cwd, &ms->gc);
	return (cwd);
}

void	ms_cd_root(t_ms *ms, char *path)
{
	char	*old_cwd;
	char	*cwd;

	old_cwd = ms_getcwd_or_error(ms);
	gc_add(old_cwd, &ms->gc);
	ms_change_directory(ms, path);
	cwd = ms_getcwd_or_error(ms);
	gc_add(cwd, &ms->gc);
	ms_set_env_variable(ms, "OLDPWD", old_cwd);
	ms_set_env_variable(ms, "PWD", cwd);
}
