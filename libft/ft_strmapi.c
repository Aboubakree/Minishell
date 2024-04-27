/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:13:45 by akrid             #+#    #+#             */
/*   Updated: 2023/11/16 23:26:01 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	int				size;
	char			*maped;

	if (s == NULL || f == NULL)
		return (NULL);
	size = ft_strlen(s);
	maped = (char *)malloc((size + 1) * sizeof(char));
	if (maped == NULL)
		return (NULL);
	i = 0;
	while (s[i])
	{
		maped[i] = f(i, s[i]);
		i ++;
	}
	maped[i] = '\0';
	return (maped);
}
