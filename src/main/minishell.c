/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 15:44:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Current main function handles:
-Making a copy of environment (implicit check of existing env var)
-Setting up the sigaction and signal handler
-Setting up and initializing the main loop (print prompt, wait for input/signals)
-In mandatory phase, both argc and argv are voided.
	In bonus they might be useful.

CHECKS: what should happen if env copy fails?
*/
int	main(int argc, char **argv, char *env[])
{
	struct sigaction	action;
	t_list				*ms_env;
	t_list				*gc;

	(void)argc;
	(void)argv;
	ms_env = NULL;
	gc = NULL;
	ms_env = ms_copy_env(&ms_env, env, &gc);
	action.sa_handler = ms_signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &action, NULL) == -1)
		ms_error_handler(&ms_env, &gc, "SIGINT sigaction error", 0);
	if (sigaction(SIGTSTP, &action, NULL) == -1)
		ms_error_handler(&ms_env, &gc, "SIGTSTP sigaction error", 0);
	ms_main_loop(&ms_env, &gc);
}
