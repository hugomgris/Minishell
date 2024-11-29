/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/29 14:16:27 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
SIGINT handler.
It re-prints the prompt in a new line and flushes stdout.
	Now it prints the composite prompt because it's able to get ms_env.
*/

void	ms_sigint_handler(void)
{
	t_list	**ms_env;
	char	*prompt;

	ms_env = ms_env_instance();
	prompt = ms_build_prompt(*ms_env);
	gc_add(prompt);
	ft_printf("\n");
	ft_putstr_fd(prompt, STDERR_FILENO);
	fflush(stdout);
}

/*
SIGTSTP handler.
Right now it ends minishell, in the future it should just suspend a process
	while minishell keeps going and taking input from user.
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
