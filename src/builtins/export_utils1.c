/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:59:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/12/24 10:28:04 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Flow control function to differentiate export cases with and w/o value.
*/
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

/*
Helper to export (set or create) variables with set values.
Calls export check function, and if passed goes to export executor.
If key doesn't pass the checks, the export error function is called.
*/
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

/*
Helper to export (set or create) variables without set values.
Key is checked to comply with export rules. 
If checks are passed, export execution is called.
Else, export error is called. 
*/
void	ms_export_without_value(t_ms *ms, char *arg)
{
	if (ms_export_check(arg))
		ms_export_ex(ms, arg, NULL);
	else
		ms_export_error(ms, arg);
}

/*
Helper function to alphabetically sort an array.
Needed for export because the printed list must be alphabetically sorted.
*/
char	**ms_sort(char **array, int (*cmp)(const void *, const void *, size_t))
{
	int		i;
	int		j;
	size_t	min_len;
	char	*temp;

	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			min_len = ft_min_strlen(array[i], array[j]);
			if (cmp(array[i], array[j], min_len) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
	return (array);
}

/*
Flow control function that builds the export output.
It builds said output mimicking bash, enclosing value in between "".
*/
char	*ms_build_export_output(t_ms *ms, char *content, char *sym)
{
	char	*key;
	char	*value;
	char	*out;

	key = ft_substr(content, 0, sym - content);
	gc_add(key, &ms->gc);
	value = ft_strdup(sym + 1);
	gc_add(value, &ms->gc);
	out = ft_strjoin(key, "=\"");
	gc_add(out, &ms->gc);
	out = ft_strjoin(out, value);
	gc_add(out, &ms->gc);
	out = ft_strjoin(out, "\"");
	gc_add(out, &ms->gc);
	return (out);
}
