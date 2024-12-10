/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:23:05 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 15:49:20 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ms_check_directory_access(t_ms *ms, char *new_path)
{
	if (access(new_path, F_OK) == -1)
		return (ms_error_handler(ms, "cd: Directory does not exist", 0), -1);
	if (ms_cd_isdirectory(ms, new_path) == 0)
		return (ms_error_handler(ms, "cd: Not a directory", 0), -1);
	if (ms_cd_isdirectory(ms, new_path) == -1)
		return (ms_error_handler(ms, "cd: Error checking directory", 0), -1);
	if (access(new_path, R_OK | X_OK) == -1)
		return (ms_error_handler(ms, "cd: Permission denied", 0), -1);
	return (0);
}

int	ms_update_oldpwd(t_ms *ms, char *current_pwd)
{
	char	*old_pwd;

	if (access(current_pwd, F_OK) != -1)
	{
		if (!ms_get_env_variable(ms, "OLDPWD="))
		{
			old_pwd = ft_strjoin("OLDPWD=", current_pwd);
			if (!old_pwd)
				return (ms_error_handler(ms, "cd: Memory alloc failed", 1), -1);
			gc_add(old_pwd, &ms->gc);
			ms_add_env_variable(ms, old_pwd);
		}
		else
		{
			ms_set_env_variable(ms, "OLDPWD", current_pwd);
		}
	}
	return (0);
}
