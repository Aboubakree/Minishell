/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 11:23:15 by akrid             #+#    #+#             */
/*   Updated: 2023/11/16 23:43:32 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	check_char(char c, const char *set)
{
	int	j;

	j = 0;
	while (set[j])
	{
		if (set[j] == c)
			return (1);
		j ++;
	}
	return (0);
}

static int	count_droped(char const *s1, char const *set, int *i, int *j)
{
	int	count;

	*i = 0;
	count = 0;
	while (s1[*i] && check_char(s1[*i], set))
	{
		count ++;
		*i = *i + 1;
	}
	*j = ft_strlen(s1) - 1;
	while (*j > *i && check_char(s1[*j], set))
	{
		count ++;
		*j = *j - 1;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	int		i;
	int		j;
	int		k;

	if (s1 == NULL || set == NULL)
		return (NULL);
	k = ft_strlen(s1) - count_droped(s1, set, &i, &j) + 1;
	trim = (char *)malloc(k * sizeof(char));
	if (trim == NULL)
		return (NULL);
	k = 0;
	while (s1[i] && i <= j)
		trim[k ++] = s1[i ++];
	trim[k] = '\0';
	return (trim);
}
