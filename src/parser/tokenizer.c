/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nponchon <nponchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:19:44 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 20:02:42 by nponchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_tokenizer(t_list *ms_env, char *str)
{
	ms_syntax_checker(str);
	str = ms_expand_variable(ms_env, str);
}
