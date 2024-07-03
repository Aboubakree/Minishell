/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_for_minishell.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 21:34:24 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/07/02 18:41:13 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	initialize_value_split2(int *j, char *quote, int *in_quote)
{
	*j = 0;
	*quote = '\0';
	*in_quote = 0;
}

void	ft_skip2(int *i, int in_quote, const char *s, char *charset)
{
	if (in_quote == 0 && check_charset(charset, s[*i]))
		(*i)++;
}

int check_charset(char *charset, char c)
{
	int i;

	i = 0;
	while(charset[i])
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_count_words(char const *str, char *charset)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && check_charset(charset, str[i]) == 1)
			i++;
		if (str[i] && !(check_charset(charset, str[i]) == 1))
		{
			count++;
			while (str[i] && !(check_charset(charset, str[i]) == 1))
				i++;
		}
	}
	return (count);
}

char	**ft_split2_helper(char **res, char const *s, char *charset, int i)
{
	int		start;
	char	quote;
	int		in_quote;
	int		j;

	initialize_value_split2(&j, &quote, &in_quote);
	update_quote_state(&in_quote, &quote, s[i]);
	while (s[i])
	{
		update_quote_state(&in_quote, &quote, s[i]);
		ft_skip2(&i, in_quote, s, charset);
		start = i;
		i--;
		while (s[++i] && (check_charset(charset, s[i]) == 0 || in_quote == 1))
			update_quote_state(&in_quote, &quote, s[i]);
		if (i <= start)
			continue ;
		res[j] = (char *)malloc((i - start + 1) * sizeof(char));
		if (!res[j])
			return (ft_free(res));
		ft_strlcpy(res[j], &s[start], i - start + 1);
		j++;
	}
	res[j] = NULL;
	return (res);
}

char	**ft_split2(char const *s, char *charset)
{
	char	**res;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	res = (char **)malloc((ft_count_words(s, charset) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	res = ft_split2_helper(res, s, charset, i);
	if (!res)
		return (ft_free(res));
	return (res);
}

