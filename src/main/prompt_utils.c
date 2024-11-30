/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:53:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 15:35:20 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ms_build_prompt(t_list **ms_env, t_list **gc)
{
	char	*username;
	char	*cwd;
	char	*prompt;
	char	*tmp;

	username = ms_get_prompt_user(ms_env, gc);
	gc_add(username, gc);
	cwd = ms_get_cwd(ms_env, gc);
	prompt = ft_strjoin("\033[0;41mminishell>\033[0m\033[1;32m ", username);
	gc_add(prompt, gc);
	if (!prompt)
		ms_error_handler(ms_env, gc, "Error: Mem alloc failed for prompt", 1);
	tmp = ft_strjoin(prompt, " ");
	gc_add(tmp, gc);
	prompt = ft_strjoin(tmp, "\033[0m\033[1;34m");
	gc_add(prompt, gc);
	tmp = ft_strjoin(prompt, cwd);
	gc_add(tmp, gc);
	tmp = ft_strjoin(tmp, "\033[0m$ ");
	gc_add(tmp, gc);
	return (tmp);
}
