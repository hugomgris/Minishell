/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isvalidsign.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 09:30:07 by hmunoz-g          #+#    #+#             */
/*   Updated: 2024/10/15 09:32:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isvalidsign(char a, char b)
{
	if ((a == '+' || a == '-') && (b != '+' && b != '-'))
		return (1);
	return (0);
}
