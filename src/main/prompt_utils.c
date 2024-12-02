/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/02 20:32:42 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_build_prompt(t_ms *ms)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	printf("Building prompt...\n");
	username = ms_get_prompt_user(ms);
	gc_add(username, &ms->gc);
	cwd = ms_get_cwd(ms);
	prompt = ft_strjoin("\033[0;41mminishell>\033[0m\033[1;32m ", username);
	gc_add(prompt, &ms->gc);
	if (!prompt)
		ms_error_handler(ms, "Error: Mem alloc failed for prompt", 1);
	tmp = ft_strjoin(prompt, " ");
	gc_add(tmp, &ms->gc);
	prompt = ft_strjoin(tmp, "\033[0m\033[1;34m");
	gc_add(prompt, &ms->gc);
	tmp = ft_strjoin(prompt, cwd);
	gc_add(tmp, &ms->gc);
	tmp = ft_strjoin(tmp, "\033[0m$ ");
	gc_add(tmp, &ms->gc);
	return (tmp);
}
