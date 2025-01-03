/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/03 09:25:31 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	TODO
	Check for possible limitation of FDs: what happens if minishell is limited
	to using only 2 fds? 1? 0?
*/

/*
Helper function to set custom colors for colored cmd calls.
When a command is called with "--color=auto", enhances output.
Uses minishell specific pallette (cyan, orange, red).
*/
void	ms_set_custom_colors(t_ms *ms)
{
	ms_set_env_variable(ms, "LS_COLORS", \
		"di=1;36:fi=0:ex=1;38;5;214:" \
		"ln=35:so=0:pi=0:bd=0:cd=0:" \
		"or=0:mi=0:su=0:sg=0");
}

void	ms_set_shlvl(t_ms *ms)
{
	char	*shlvl;

	if (ms_get_env_variable(ms, "SHLVL"))
	{
		shlvl = ft_itoa(ft_atoi(ms_get_env_variable(ms, "SHLVL")) + 1);
		gc_add(shlvl, &ms->gc);
		ms_set_env_variable(ms, "SHLVL", shlvl);
	}
	else
		ms_add_env_variable(ms, "SHLVL", "1");
}

/*
Initialization of the ms minishell struct.
Stores fixed values of env variables if existing.
It sets up the necessary NULLs and handles:
	-Making a copy of the env variables
	-Storing a home reference in the ms struct
	-Storing a user reference in the ms struct
*/
void	ms_init(t_ms *ms, char **env)
{
	ms->ms_env = NULL;
	ms->gc = NULL;
	ms->user = NULL;
	ms->tok = NULL;
	ms->tokens = NULL;
	ms->redir_tokens = NULL;
	ms->filtered_tokens = NULL;
	ms->exit_status = 0;
	ms->heredoc_fd = -1;
	ms->ms_env = ms_copy_env(ms, env);
	ms->home = ms_make_home_ref(ms, env);
	ms->user = ms_get_prompt_user(ms);
	ms_set_shlvl(ms);
	ms_set_custom_colors(ms);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	read_history(0);
}

/*
Magic sparks, minishell begins.
Calls ms struct initialization function.
Sets up signal handling.
Launches the minishell loop.
*/
int	main(int argc, char **argv, char *env[])
{
	struct sigaction	action;
	t_ms				ms;

	(void)argc;
	(void)argv;
	ms_init(&ms, env);
	action.sa_flags = SA_RESTART;
	action.sa_handler = ms_signal_handler;
	if (sigaction(SIGINT, &action, NULL) == -1)
		ms_error_handler(&ms, "SIGINT sigaction error", 0);
	ms_main_loop(&ms);
}
