/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/14 15:05:22 by hmunoz-g         ###   ########.fr       */
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
	ms->wc = NULL;
	ms->exit_status = 0;
	ms->heredoc_fd = -1;
	ms->ms_env = ms_copy_env(ms, env);
	ms->home = ms_make_home_ref(ms, env);
	ms->user = ms_get_prompt_user(ms);
	ms->export_only = NULL;
	ms->expr_tree = NULL;
	ms_set_shlvl(ms);
	ms_set_custom_colors(ms);
	read_history(0);
}

/*
Flow control function to set up the signal handlers for the whole shell.
*/
void	ms_setup_signal_handlers(t_ms *ms)
{
	struct sigaction	action_int;
	struct sigaction	action_quit;

	action_int.sa_flags = SA_RESTART;
	action_int.sa_handler = ms_signal_handler;
	sigemptyset(&action_int.sa_mask);
	action_quit.sa_flags = SA_RESTART;
	action_quit.sa_handler = SIG_IGN;
	sigemptyset(&action_quit.sa_mask);
	if (sigaction(SIGINT, &action_int, NULL) == -1)
		ms_error_handler(ms, "SIGINT sigaction error", 0);
	if (sigaction(SIGQUIT, &action_quit, NULL) == -1)
		ms_error_handler(ms, "SIGQUIT sigaction error", 0);
	signal(SIGTSTP, SIG_IGN);
}

/*
Magic sparks, minishell begins.
Calls ms struct initialization function.
Sets up signal handling.
Launches the minishell loop.
*/
int	main(int argc, char **argv, char *env[])
{
	t_ms				ms;

	(void)argc;
	(void)argv;
	ms_init(&ms, env);
	ms_setup_signal_handlers(&ms);
	ms_main_loop(&ms);
}
