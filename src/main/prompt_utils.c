/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 09:05:01 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Stores a home reference in the ms minishell struct upon initialization.
If home can't be built (init from non-existing dir), returns NULL. 
*/
char	*ms_make_home_ref(t_ms *ms, char **env)
{
	char	*cwd;
	int		i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "HOME", 4))
			return (env[i] + 5);
		i++;
	}
	cwd = NULL;
	cwd = getcwd(cwd, PATH_MAX);
	if (!cwd)
		return (NULL);
	gc_add(cwd, &ms->gc);
	cwd = ft_strchr(cwd, '/');
	gc_add(cwd, &ms->gc);
	cwd = ft_substr(cwd, 0, ft_strchr_n(cwd, '/', 3) - cwd);
	gc_add(cwd, &ms->gc);
	return (cwd);
}

/*
Main prompt building function.
Calls a set of different functions that sequentially build a composite prompt.
Minishell's implemented prompt contains:
	-Minishell> signifier.
	-Username and hostname
	-CWD
	-$ symbol to indicate end of prompt.
Colors are implemented to differentiate it from bash, zsh and other shell calls.
	(Minishell can be called from bash and viceversa; same with others)
*/
char	*ms_build_prompt(t_ms *ms)
{
	char	*user;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	user = ms_get_prompt_user(ms);
	cwd = ms_get_cwd(ms);
	prompt = ft_strjoin("\033[0;41mminishell>\033[0m\033[1;38;5;214m ", user);
	gc_add(prompt, &ms->gc);
	if (!prompt)
		ms_error_handler(ms, "Error: Mem alloc failed for prompt", 1);
	prompt = ft_strjoin(prompt, "\033[0m\033[1;36m");
	gc_add(prompt, &ms->gc);
	tmp = ft_strjoin(prompt, cwd);
	gc_add(tmp, &ms->gc);
	tmp = ft_strjoin(tmp, "\033[0m$ ");
	gc_add(tmp, &ms->gc);
	return (tmp);
}
