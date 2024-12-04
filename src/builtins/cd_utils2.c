/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:30:06 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/04 19:18:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_pop_from_path(char *path)
{
	int		len;

	ft_printf("prepop path:%s\n", path);
	len = ft_strlen(path);
	if (len == 1 && path[0] == '/')
		return (path);
	while (len > 0 && path[len - 1] != '/')
		len--;
	if (len > 1)
		len--;
	path[len] = '\0';
	ft_printf("popped path:%s\n", path);
	return (path);
}

char	*ms_normalize_path(t_ms *ms, char *path)
{
	char	cwd[PATH_MAX];
	char	*normalized;
	char	**components;
	int		i;

	ft_printf("normalizer got: %s\n", path);
	if (path[0] == '/')
		normalized = ft_strdup("/");
	else
	{
		if (!getcwd(cwd, PATH_MAX))
			ms_error_handler(ms, "Minishell: cd: unable to get cwd", 0);
		normalized = ft_strdup(cwd);
	}
	gc_add(normalized, &ms->gc);
	components = ft_split(path, '/');
	i = 0;
	while (components[i])
	{
		if (!ft_strncmp(components[i], "..", 2))
			ms_pop_from_path(normalized);
		else if (ft_strncmp(components[i], ".", 1) != 0)
		{
			if (ms_join_paths(normalized, components[i], &normalized) == -1)
			{
				ft_free(components);
				return (NULL);
			}
		}
		i++;
	}
	ft_free(components);
	if (ft_strlen(normalized) == 0)
		normalized = ft_strdup("/");
	ft_printf("normalized: %s\n", normalized);
	return (normalized);
}
