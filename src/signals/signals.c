/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/13 10:31:32 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
This is a get-setter for the only global variable allowed in Minishell.
The variable is used to indicate the reception of a signal and its number.
To avoid splitting this into two functions (get and set), an action int is used.
1 = set; 0 = get.
*/
int	ms_get_set(int action, int val)
{
	static int	var;

	if (action == 1)
		var = val;
	return (var);
}

/*
SIGINT handler.
Reprints the prompt in a new line, interrumpting a process.
*/

void	ms_sigint_handler(void)
{
	int	in_child;

	in_child = ms_get_set(0, 0);
	if (in_child)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		return ;
	}
	ms_get_set(1, 0);
	ft_putstr_fd("\n", STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
SIGTSTP handler.
TODO: when jobs and processes are implemented, come back to build this.
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
