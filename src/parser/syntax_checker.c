/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/27 18:00:39 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	TODO

	- Check for quotes (opened and closed)
	Enclosing characters in single-quotes ( '' ) shall preserve
	the literal value of each character within the single-quotes. 
	A single-quote cannot occur within single-quotes.

	- Check for pipes (no pipe w/o command before or after)
	
	- Check for special characters ('\', '\n', ';' etc.)
*/

void	ms_checkquotes(char *str, char c)
{
	int	i;
	int	flag;

	i = -1;
	flag = 0;
	while (str[++i])
	{
		if (str[i] == c && flag == 1)
			flag = 0;
		else if (str[i] == c && flag == 0)
			flag = 1;
	}
	if (flag)
		ft_putstr_fd("Wrong pipe: no command before \"|\"\n", 2);
}

void	ms_checkspecialchar(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 59 || str[i] == 92)
			ft_putstr_fd("Invalid character: ';' '\\'\n", 2);
	}
}

void	ms_checkpipes(char *str)
{
	int	i;

	i = -1;
	while (ft_isspace(str[++i]))
		;
	if (str[i] == '|')
	{
		ft_putstr_fd("Wrong pipe: no command before \"|\"\n", 2);
		return ;
	}
	while (str[i++])
		;
	if (str[i - 2] == '|')
		ft_putstr_fd("Wrong pipe: no command after \"|\"\n", 2);
}

void	ms_syntax_checker(char *str)
{
	ms_checkquotes(str, 39);
	ms_checkquotes(str, 34);
	ms_checkpipes(str);
	ms_checkspecialchar(str);
}
