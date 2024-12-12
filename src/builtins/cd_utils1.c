/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:49:43 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/12 09:47:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Bypass function that expands the initial portion of a path starting with '~'.
'~' is a substitution character for /home/user,
	so this is a normalizing step that makes minishell able to accept
		cd paths with '~' abbreviation.
*/
char	*ms_expand_tilde(t_ms *ms, char *path)
{
	char	*expanded_path;

	if (path[0] == '~')
	{
		expanded_path = ft_strjoin(ms->home, path + 1);
		if (!expanded_path)
			ms_error_handler(ms, "Error: Mem alloc failed", 1);
		gc_add(expanded_path, &ms->gc);
		return (expanded_path);
	}
	return (path);
}

/*
Bypass function to join paths in case of inserted '..' or '.' in argument path.
*/
int	ms_join_paths(t_ms *ms, char *cwd, char *path, char **new_path)
{
	size_t	cwd_len;
	size_t	path_len;
	size_t	total_len;

	cwd_len = ft_strlen(cwd);
	path_len = ft_strlen(path);
	total_len = cwd_len + path_len + 2;
	*new_path = malloc(total_len);
	if (!*new_path)
		return (-1);
	gc_add(*new_path, &ms->gc);
	ft_strlcpy(*new_path, cwd, total_len);
	ft_strlcat(*new_path, "/", total_len);
	ft_strlcat(*new_path, path, total_len);
	return (0);
}

/*
The heart of the cd builtin command.
Attempts to change directory to path received as argument.
Handles change attempt error both with output and int return.
Handles the necessary changes to PWD and OLDPWD ms_env entries, if set.
*/
int	ms_change_directory(t_ms *ms, char *new_path)
{
	char	*current_pwd;

	if (ms_check_directory_access(ms, new_path) == -1)
		return (-1);
	current_pwd = ms_get_env_variable(ms, "PWD");
	if (!current_pwd)
	{
		current_pwd = ms_get_cwd(ms);
		if (!current_pwd)
			current_pwd = "";
	}
	if (ms_update_oldpwd(ms, current_pwd) == -1)
		return (-1);
	if (chdir(new_path) == -1)
		return (ms_error_handler(ms, "cd: Unable to change directory", 0), -1);
	ms_set_env_variable(ms, "PWD", new_path);
	return (0);
}

/*
Flow control function to check for cwd availability.
Outputs error if cwd ins unreachable (being in a non-existing dir case).
*/
char	*ms_getcwd_or_error(t_ms *ms)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ms_error_handler(ms, "cd: Couldn't get CWD", 0);
		return (NULL);
	}
	gc_add(cwd, &ms->gc);
	return (cwd);
}

/*
Handles cding to root directory (case cd /).
*/
void	ms_cd_root(t_ms *ms, char *path)
{
	if (ms_change_directory(ms, path) == -1)
		return ;
}
