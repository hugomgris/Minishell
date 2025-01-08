/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:33:16 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/01/08 14:23:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static const char	*ft_skip_spaces(const char *str)
{
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	return (str);
}

static int	ft_check_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

static long long	ft_handle_overflow(char **endptr, const char *str, int sign)
{
	if (endptr)
		*endptr = (char *)str;
	errno = ERANGE;
	if (sign == 1)
		return (LLONG_MAX);
	return (LLONG_MIN);
}

long long	ft_strtoll(const char *str, char **endptr)
{
	long long	result;
	int			sign;

	result = 0;
	str = ft_skip_spaces(str);
	sign = ft_check_sign(&str);
	while (*str >= '0' && *str <= '9')
	{
		if (result > LLONG_MAX / 10 || (result == LLONG_MAX / 10
				&& (*str - '0') > LLONG_MAX % 10))
			return (ft_handle_overflow(endptr, str, sign));
		result = result * 10 + (*str - '0');
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
