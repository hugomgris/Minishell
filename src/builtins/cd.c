/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/29 14:42:31 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handler function. Calls chdir to go to home dir. 
*/
void	ms_cd_home(t_list *ms_env)
{
	char	*home;
	char	*inv_home;

	home = ms_get_env_variable(ms_env, "HOME=");
	gc_add(home);
	if (chdir(home) == -1)
	{
		inv_home = ft_strjoin("minishell: cd: ", home);
		gc_add(inv_home);
		inv_home = ft_strjoin(inv_home, ": No such file or directory");
		gc_add(inv_home);
	}
}

/*
Handler function. Goes one directory back.
Needs to handle the endline (root/home dir).
*/
void	ms_cd_back(t_list *ms_env)
{
	(void)ms_env;
}

/*
Handler function. Calls chdir to a relative path.
*/
void	ms_cd_dir(t_list *ms_env)
{
	(void)ms_env;
}

/*
cd builtin command hub.
Takes path to go to as argument (cd command itself pre-checked in executor).
Handles the different cases that the regular cd should tend to.
In case of absolute path, tries to chdir to said path.
*/
void	ms_cd(char *path)
{
	t_list	**ms_env;
	char	*inv_msg;

	ms_env = ms_env_instance();
	if (!path || path == NULL || (path[0] == '~' && !path[1]))
		ms_cd_home(*ms_env);
	else if (path[0] == '-' && !path[1])
		ms_cd_back(*ms_env);
	else if (path[0] == '/')
	{
		if (chdir(path) == -1)
		{
			inv_msg = ft_strjoin("minishell: cd: ", path);
			gc_add(inv_msg);
			inv_msg = ft_strjoin(inv_msg, ": No such file or directory");
			gc_add(inv_msg);
			ms_error_handler(inv_msg, 0);
		}
	}
	else
		ms_cd_dir(*ms_env);
}
