/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 19:39:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/10/09 17:18:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <aio.h>

int	reject_char(char c, const char *reject)
{
	int	i;

	i = 0;
	while (reject[i])
	{
		if (reject[i] == c)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_strcspn(const char *s, const char *reject)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (!reject_char(s[i], reject))
			count++;
		else
			break ;
		i++;
	}
	return (count);
}

/*#include <stdio.h>
int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		size_t c = ft_strcspn(argv[1], argv[2]);
		printf("%zu", c);
	}
	return (0);
}*/
