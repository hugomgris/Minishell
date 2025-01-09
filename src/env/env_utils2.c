/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 10:33:54 by nponchon          #+#    #+#             */
/*   Updated: 2025/01/09 10:37:21 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Looks for possible locations of the utmp file on the system,
	and checks for reading permission.
*/
char	*ms_find_utmp(void)
{
	char	*pathname;

	pathname = NULL;
	if (!access("/etc/utmp", R_OK))
		pathname = "/etc/utmp";
	else if (!access("/var/adm/utmp", R_OK))
		pathname = "/var/adm/utmp";
	else if (!access("/var/run/utmp", R_OK))
		pathname = "/var/run/utmp";
	return (pathname);
}

/*
Prompt helper function used as fallback if username retrieval fails.
Tries to read from utmp file and extract the current logged user.
*/
char	*ms_username_from_utmp(t_ms *ms)
{
	int			fd;
	char		*line;
	char		*username;
	char		*token;

	line = NULL;
	username = "unknown";
	fd = open(ms_find_utmp(), O_RDONLY);
	if (fd == -1)
		return (username);
	line = get_next_line(fd);
	if (line)
	{
		ft_strtok(line, ":");
		token = ft_strtok(0, ":");
		if (token)
		{
			username = ft_strdup(token + 1);
			close(fd);
			free(line);
			gc_add(username, &ms->gc);
		}
	}
	return (username);
}
