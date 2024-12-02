/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/30 16:47:26 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
SIGINT handler.
It re-prints the prompt in a new line and flushes stdout.
*/

void	ms_sigint_handler(void)
{
	ft_putstr_fd("\nshittyshell$ ", STDERR_FILENO);
	fflush(stdout);
}

/*
SIGTSTP handler.
*/
void	ms_sigtstp_handler(void)
{
	write(STDOUT_FILENO, "\nProcess suspended. Type 'fg' to resume.\n", 41);
	signal(SIGTSTP, SIG_DFL);
	kill(getpid(), SIGTSTP);
}

/*
Signal hub.
Catches signals and redirects them to their respective handler functions.
*/
void	ms_signal_handler(int signal)
{
	if (signal == SIGINT)
		ms_sigint_handler();
	else if (signal == SIGTSTP)
		ms_sigtstp_handler();
}
