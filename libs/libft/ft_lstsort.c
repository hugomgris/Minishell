/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:11:21 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/07 10:09:57 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstsort(t_list *l, int (*cmp)(const void *, const void *, size_t))
{
	char	*aux;
	int		len[2];
	size_t	min_len;
	t_list	*start;

	start = l;
	while (l && l->next)
	{
		len[0] = ft_strlen(l->content) + 1;
		len[1] = ft_strlen(l->next->content) + 1;
		if (len[0] < len[1])
			min_len = len[0];
		else
			min_len = len[1];
		if ((*cmp)(l->content, l->next->content, min_len) > 0)
		{
			aux = l->content;
			l->content = l->next->content;
			l->next->content = aux;
			l = start;
		}
		else
			l = l->next;
	}
	return (start);
}
