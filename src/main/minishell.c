/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 18:10:15 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Current main function handles:
-Making a copy of environment (implicit check of existing env var)
-Setting up the sigaction and signal handler
-Setting up and initializing the main loop (print prompt, wait for input/signals)
-In mandatory phase, both arms.gc and argv are voided.
	In bonus they might be useful.

TODO: take as many main lines as possible to the init function
*/

void	ms_init(t_ms *ms, char **env)
{
	ms->ms_env = NULL;
	ms->gc = NULL;
	ms->user = NULL;
	ms->tokens = NULL;
	ms->exit_status = 0;
	ms->ms_env = ms_copy_env(ms, env);
	ms->home = ms_make_home_ref(ms, env);
	ms->user = ms_get_prompt_user(ms);
}

int	main(int argc, char **argv, char *env[])
{
	struct sigaction	action;
	t_ms				ms;

	(void)argc;
	(void)argv;
	ms_init(&ms, env);
	action.sa_handler = ms_signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &action, NULL) == -1)
		ms_error_handler(&ms, "SIGINT sigaction error", 0);
	if (sigaction(SIGTSTP, &action, NULL) == -1)
		ms_error_handler(&ms, "SIGTSTP sigaction error", 0);
	ms_main_loop(&ms);
}
