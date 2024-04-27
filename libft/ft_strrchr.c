/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:32:45 by akrid             #+#    #+#             */
/*   Updated: 2023/11/16 23:34:27 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	char	*temp;

	temp = 0;
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			temp = (char *)s;
		s ++;
	}
	if ((unsigned char)c == (unsigned char) '\0')
		return ((char *)s);
	return (temp);
}
