/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:30:06 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 14:08:06 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_pop_from_path(char *path)
{
	int		len;

	len = ft_strlen(path);
	if (len == 1 && path[0] == '/')
		return (path);
	while (len > 0 && path[len - 1] != '/')
		len--;
	if (len > 1)
		len--;
	path[len] = '\0';
	return (path);
}

char	*ms_get_cwd_or_root(char *path)
{
	char	cwd[PATH_MAX];

	if (path[0] == '/')
		return (ft_strdup(""));
	if (!getcwd(cwd, PATH_MAX))
		return (NULL);
	return (ft_strdup(cwd));
}

char	*ms_process_component(char *normalized, char *component, t_ms *ms)
{
	if (!ft_strncmp(component, "..", 2))
		ms_pop_from_path(normalized);
	else if (ft_strncmp(component, ".", 1) != 0)
	{
		if (ms_join_paths(ms, normalized, component, &normalized) == -1)
			return (NULL);
	}
	if (!ft_strncmp(normalized, "//", 2))
	{
		normalized = ft_strdup(ft_strchr_n(normalized, '/', 2));
		gc_add(normalized, &ms->gc);
	}
	return (normalized);
}

char	*ms_normalize_path(t_ms *ms, char *path)
{
	char	*normalized;
	char	**components;
	int		i;

	normalized = ms_get_cwd_or_root(path);
	if (!normalized)
		return (NULL);
	gc_add(normalized, &ms->gc);
	i = 0;
	components = ft_split(path, '/');
	while (components[i])
	{
		normalized = ms_process_component(normalized, components[i], ms);
		if (!normalized)
		{
			ft_free(components);
			return (NULL);
		}
		i++;
	}
	ft_free(components);
	if (ft_strlen(normalized) == 0)
		normalized = ft_strdup("/");
	return (normalized);
}

void	ms_cd_ascend(t_ms *ms)
{
	char	*cwd;

	cwd = ms_get_cwd(ms);
	if (!ft_strncmp(cwd, "?", 1))
	{
		ms_error_handler(ms, \
			"cd: cannot find current nor parent directory: Moving to root", 0);
		cwd = ft_strdup("/");
		gc_add(cwd, &ms->gc);
	}
	else
	{
		cwd = ft_strdup(ms->prompt);
		gc_add(cwd, &ms->gc);
		cwd = ft_strchr(cwd, '~');
		cwd = ms_expand_tilde(ms, cwd);
	}
	gc_add(cwd, &ms->gc);
	while (chdir(cwd) == -1)
	{
		cwd = ft_substr(cwd, 0, ft_strrchr(cwd, '/') - cwd);
		gc_add(cwd, &ms->gc);
	}
	if (ms_get_env_variable(ms, "PWD="))
		ms_set_env_variable(ms, "PWD", cwd);
}
