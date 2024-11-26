/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/26 12:27:45 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Current main function handles:
-Setting up the sigaction and signal handler
-Setting up and initializing the main loop (print prompt, wait for input/signals)
*/
int	main(void)
{
	struct sigaction	action;

	action.sa_handler = ms_signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &action, NULL) == -1)
		ms_error_handler("SIGINT sigaction error");
	if (sigaction(SIGTSTP, &action, NULL) == -1)
		ms_error_handler("SIGTSTP sigaction error");
	sigemptyset(&action.sa_mask);
	ms_main_loop();
}
