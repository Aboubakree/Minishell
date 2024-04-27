/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 21:57:27 by akrid             #+#    #+#             */
/*   Updated: 2023/11/17 11:42:30 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*copy_sub(char	*sub, const char *s, size_t start, size_t max_size)
{
	size_t	i;

	sub = (char *)malloc((max_size + 1) * sizeof(char));
	if (sub == NULL)
		return (NULL);
	i = 0;
	while (i < max_size)
	{
		sub[i] = s[start + i];
		i ++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*sub;

	sub = NULL;
	if (s == NULL)
		return (copy_sub(sub, s, 0, 0));
	s_len = ft_strlen(s);
	if (len <= s_len - (size_t)start && (size_t)start <= s_len)
		return (copy_sub(sub, s, (size_t)start, len));
	else if ((size_t)start <= s_len)
		return (copy_sub(sub, s, (size_t)start, s_len - (size_t)start));
	return (copy_sub(sub, s, 0, 0));
}
