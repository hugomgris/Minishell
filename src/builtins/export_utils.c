/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:59:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/10 18:04:18 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ms_process_export_arg(t_ms *ms, char *arg)
{
	char	*sign;

	sign = ft_strchr(arg, '=');
	if (sign)
	{
		ms_export_with_value(ms, arg, sign);
	}
	else
		ms_export_without_value(ms, arg);
}

void	ms_export_with_value(t_ms *ms, char *arg, char *sign)
{
	char	*key;
	char	*value;

	key = ft_substr(arg, 0, sign - arg);
	value = ft_strdup(sign + 1);
	gc_add(key, &ms->gc);
	gc_add(value, &ms->gc);
	if (ms_export_check(key))
		ms_export_ex(ms, key, value);
	else
		ms_export_error(ms, arg);
}

void	ms_export_without_value(t_ms *ms, char *arg)
{
	if (ms_export_check(arg))
		ms_export_ex(ms, arg, NULL);
	else
		ms_export_error(ms, arg);
}

t_list	*ms_sort(t_list *lst, int (*cmp)(const void *, const void *, size_t))
{
	char	*aux;
	int		len[2];
	size_t	min_len;
	t_list	*start;

	start = lst;
	while (lst && lst->next)
	{
		len[0] = ft_strlen(lst->content) + 1;
		len[1] = ft_strlen(lst->next->content) + 1;
		if (len[0] < len[1])
			min_len = len[0];
		else
			min_len = len[1];
		if ((*cmp)(lst->content, lst->next->content, min_len) > 0)
		{
			aux = lst->content;
			lst->content = lst->next->content;
			lst->next->content = aux;
			lst = start;
		}
		else
			lst = lst->next;
	}
	return (start);
}
