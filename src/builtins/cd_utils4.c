/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:23:05 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/02 19:02:52 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Helper function to check cd path's aiming dir availability.
Handles different possible error cases.
*/
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

/*
Helper function that updates the OLDPWD entry in ms_env struct.
If OLDPWD is unset, this makes it's resetting possible.
It is a tool function to remake the OLDPWD in an unset env case.
	I.E., if unset OLDPWD, this makes it reset possible after 2 cds.
*/
int	ms_update_oldpwd(t_ms *ms, char *current_pwd)
{
	if (access(current_pwd, F_OK) != -1)
	{
		if (!ms_get_env_variable(ms, "OLDPWD"))
		{
			ms_add_env_variable(ms, "OLDPWD", current_pwd);
		}
		else
		{
			ms_set_env_variable(ms, "OLDPWD", current_pwd);
		}
	}
	return (0);
}
