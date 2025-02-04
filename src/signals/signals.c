/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/14 15:05:23 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
This is a get-setter for the only global variable allowed in Minishell.
The variable is used to indicate the reception of a signal and its number.
To avoid splitting this into two functions (get and set), an action int is used.
Actions:
	0 - GET
	1 - SET
States:
	0 - Normal shell
	1 - In child process
	2 - In heredoc input
	3 - Heredoc interrupted by signal
*/
int	ms_get_set(int action, int val)
{
	if (action == SET)
		g_var = val;
	return (g_var);
}

/*
SIGINT handler.
Interrupts a process and returns control to user.
(prints minishell prompt and waits for new user input).
*/
void	ms_sigint_handler(void)
{
	int	state;

	state = ms_get_set(GET, 0);
	if (state == SHELL_CHILD_PROCESS)
	{
		ms_get_set(SET, SHELL_HEREDOC_INTERRUPTED);
		ft_putstr_fd("\n", STDERR_FILENO);
		return ;
	}
	else if (state == SHELL_HEREDOC)
	{
		ms_get_set(SET, SHELL_HEREDOC_INTERRUPTED);
		ft_putstr_fd("\n", STDERR_FILENO);
		close(STDIN_FILENO);
		rl_replace_line("", 0);
		rl_done = 1;
		return ;
	}
	ms_get_set(SET, SHELL_NORMAL);
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ms_sigquit_handler(void)
{
	int	state;

	state = ms_get_set(GET, 0);
	if (state == SHELL_CHILD_PROCESS)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		return ;
	}
}

void	ms_reset_signal_handlers(t_ms *ms)
{
	struct sigaction	action_quit;

	action_quit.sa_flags = SA_RESTART;
	action_quit.sa_handler = ms_signal_handler;
	sigemptyset(&action_quit.sa_mask);
	if (sigaction(SIGQUIT, &action_quit, NULL) == -1)
		ms_error_handler(ms, "SIGQUIT sigaction error", 0);
}

/*
Signal hub.
Catches signals and redirects them to their respective handler functions.
*/
void	ms_signal_handler(int signal)
{
	if (signal == SIGINT)
		ms_sigint_handler();
	else if (signal == SIGQUIT)
		ms_sigquit_handler();
}
