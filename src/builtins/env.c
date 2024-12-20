/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/18 16:53:57 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
ENV builtin command handler.
Prints the env variables with set up values inside the ms_env struct.
(Existing variables with no value are handled by EXPORT builtin)
*/
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

/*
PWD builtin command handler.
Retrieves the CWD and prints it, with 3 possible cases:
	-If env is set and has a PWD entry, returns its value.
	-If this fails, tries to get the cwd by calling getcwd.
	-If no PWD/CWD can be retrieved, outputs a non-critical error.
*/
void	ms_pwd(t_ms *ms)
{
	char	*cwd;

	if (ms_get_env_variable(ms, "PWD"))
	{
		ft_printf("%s\n", ms_get_env_variable(ms, "PWD"));
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

/*
UNSET builtin command handler.
Searches for variable name in env list (ms_env).
If found, pops it out of the list (handling links).
*/
void	ms_unset(t_ms *ms)
{
	t_list	*current;
	t_list	*previous;
	t_list	*tokens;

	tokens = ms->exec_tokens[0];
	if (!ms->ms_env || !tokens->next || ft_strchr(tokens->next->content, '='))
		return ;
	current = ms->ms_env;
	previous = NULL;
	while (current)
	{
		if (!ft_strncmp(current->content,
				tokens->next->content, ft_strlen(tokens->next->content)))
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
