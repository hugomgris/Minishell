/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:42:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/20 18:07:22 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This file contains helper/Flow-control functions for exec tools cleanup:
	-ms_executor_cleanup
*/

#include "../../includes/minishell.h"

void	ms_executor_cleanup(t_ms *ms, char	**env)
{
	ft_lstclear(&ms->tokens, free);
	ft_free(ms->exec_chunks);
	ft_free(env);
}
