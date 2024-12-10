/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 08:49:51 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/10/07 08:59:59 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <aio.h>

size_t	ft_strspn(const char *s, const char *accept)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] && s[i] != accept[0])
		i++;
	while (s[i] && s[i] == accept[j])
	{
		i++;
		j++;
	}
	return (j);
}

/*#include <stdio.h>
int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		size_t count = ft_strspn(argv[1], argv[2]);
		printf("%zu", count);
	}
	printf("\n");
}*/
