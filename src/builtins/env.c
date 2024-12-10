/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 17:19:30 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_env(t_ms *ms)
{
	t_list	*current;

	current = ms->ms_env;
	while (current)
	{
		if (ft_strchr((char *)current->content, '='))
			ft_printf("%s\n", (char *)current->content);
		current = current->next;
	}
}

void	ms_pwd(t_ms *ms)
{
	char	*cwd;

	if (ms_get_env_variable(ms, "PWD="))
	{
		ft_printf("%s\n", ms_get_env_variable(ms, "PWD="));
		return ;
	}
	else
	{
		cwd = NULL;
		cwd = getcwd(cwd, PATH_MAX);
		if (cwd)
		{
			gc_add(cwd, &ms->gc);
			ft_printf("%s\n", cwd);
			return ;
		}
	}
	ms_error_handler(ms, "pwd: Cannot retrieve current directory", 0);
}

void	ms_unset(t_ms *ms, char **args)
{
	t_list	*current;
	t_list	*previous;

	if (!ms->ms_env || !args[1] || ft_strchr(args[1], '='))
		return ;
	current = ms->ms_env;
	previous = NULL;
	while (current)
	{
		if (!ft_strncmp(current->content, args[1], ft_strlen(args[1])))
		{
			if (previous == NULL)
				ms->ms_env = current->next;
			else
				previous->next = current->next;
			ft_lstdelone(current, free);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
