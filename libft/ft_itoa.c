/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:38:48 by akrid             #+#    #+#             */
/*   Updated: 2023/11/17 10:31:57 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	reverse(long nbr, int *size, short *ng)
{
	long	temp;

	temp = 0;
	*size = 0;
	*ng = 0;
	if (nbr < 0)
	{
		nbr = -nbr;
		*ng = 1;
	}
	while (nbr > 9)
	{
		temp *= 10;
		temp += nbr % 10;
		nbr /= 10;
		*size += 1;
	}
	temp *= 10;
	temp += nbr % 10;
	*size += 1;
	return (temp);
}

char	*ft_itoa(int n)
{
	long	temp;
	short	ng;
	int		size;
	char	*result;

	temp = reverse(n, &size, &ng);
	result = (char *)malloc((size + ng + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	n = 0;
	if (ng)
		result[n ++] = '-';
	while (n < size + ng)
	{
		result[n ++] = (temp % 10) + '0';
		temp /= 10;
	}
	result[n] = '\0';
	return (result);
}
