/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 13:17:35 by akrid             #+#    #+#             */
/*   Updated: 2023/11/16 23:42:26 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;
	int	b;

	i = 0;
	count = 0;
	b = 1;
	while (s[i])
	{
		if (s[i] != c && b)
		{
			count ++;
			b = 0;
		}
		if (s[i] == c && b == 0)
			b = 1;
		i ++;
	}
	return (count);
}

static int	count_len(char const *s, char c, int *i)
{
	int	count;

	count = 0;
	while (s[*i] == c)
		*i = *i + 1;
	while (s[*i])
	{
		if (s[*i] != c)
			count ++;
		if (s[*i] == c)
			return (count);
		*i = *i + 1;
	}
	return (count);
}

static char	**allocat_mem(char const *s, char c)
{
	char	**temp;
	int		size;
	int		i;
	int		k;

	size = count_words(s, c);
	temp = (char **)malloc((size + 1) * sizeof(char *));
	if (temp == NULL)
		return (NULL);
	k = 0;
	i = 0;
	while (k < size)
	{
		temp[k] = malloc((count_len(s, c, &i) + 1) * sizeof(char));
		if (temp[k] == NULL)
		{
			while (k > 0)
				free(temp[--k]);
			free(temp);
			return (NULL);
		}
		k ++;
	}
	temp[k] = NULL;
	return (temp);
}

static char	**copy_splited(char **split, char const *s, char c, int j)
{
	int	i;
	int	k;

	i = 0;
	while (split[i])
	{
		k = 0;
		while (s[j] == c)
			j ++;
		while (s[j])
		{
			if (s[j] != c)
				split[i][k ++] = s[j];
			if (s[j] == c)
			{
				split[i][k] = '\0';
				break ;
			}
			j ++;
		}
		if (s[j] == '\0')
			split[i][k] = '\0';
		i ++;
	}
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		j;

	if (s == NULL)
		return (NULL);
	j = 0;
	split = allocat_mem(s, c);
	if (split == NULL)
		return (NULL);
	split = copy_splited(split, s, c, j);
	return (split);
}
