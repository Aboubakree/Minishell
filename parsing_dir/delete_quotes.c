/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   delete_quotes.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/29 16:08:13 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/30 19:45:33by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void ft_remove_2_chars(char *str, int index1, int index2)
{
    int len = 0;

	if (str == NULL)
		return;
	len = ft_strlen(str);	
	int i ;
	if (index1 > len || index2 > len || index1 < 0 || index2 < 0)
		return;
	i = index1;
	while(i < len)
	{
		str[i] = str[i + 1];
		i++;
	}
	if (index2 > index1)
		index2--;
	i = index2;
	while(i < len - 1)
	{
		str[i] = str[i + 1];
		i++;
	}
	str[len - 2] = '\0';
}


// void delete_quotes_from_string(char *str)
// {
//     if (str == NULL)
//         return;

//     int len = ft_strlen(str);
//     int i = 0;
//     int j = 0;

//     while (i < len)
//     {
//         if (str[i] == '"' || str[i] == '\'')
//         {
//             char quote = str[i];
//             i++;
//             while (i < len && str[i] != quote)
//             {
//                 str[j++] = str[i++];
//             }
//             if (i < len)
// 			{
//                 i++;  // Skip the closing quote
// 			}
//         }
//         else
//         {
//             str[j++] = str[i++];
//         }
//     }
//     str[j] = '\0';
// }


// void delete_quotes_from_string(char *str)
// {
//     if (str == NULL)
//         return;

//     int len = ft_strlen(str);
//     int i = 0;
//     int j = 0;

//     while (i < len)
//     {
//         if ((str[i] == '"' || str[i] == '\'') && i + 1 < len)
//         {
//             char quote = str[i];
//             int start = i;
//             i++;
//             while (i < len && str[i] != quote)
//             {
//                 i++;
//             }
//             if (i < len && str[i] == quote)
//             {
//                 // Found matching quote, skip both quotes
//                 i++;
//                 while (start + 1 < i - 1)
//                 {
//                     str[j++] = str[++start];
//                 }
//             }
//             else
//             {
//                 // No matching quote found, keep the opening quote
//                 str[j++] = str[start];
//                 i = start + 1;
//             }
//         }
//         else
//         {
//             str[j++] = str[i++];
//         }
//     }
//     str[j] = '\0';
// }
void delete_quotes_from_string_helper(char *str, int *k,
    int *in_single_quote, int *in_double_quote)
{
    int j = 0;
    while (str[j])
    {
        if (str[j] == '\'' && !*in_double_quote)
        {
            if (*in_single_quote)
            {
                *in_single_quote = 0;
            }
            else
            {
                *in_single_quote = 1;
                // Check for matching closing quote
                int temp_j = j + 1;
                while (str[temp_j] && str[temp_j] != '\'')
                    temp_j++;
                if (!str[temp_j])
                {
					// No matching closing quote, keep this quote
                    str[(*k)++] = str[j];
                }
            }
        }
        else if (str[j] == '\"' && !*in_single_quote)
        {
            if (*in_double_quote)
            {
                *in_double_quote = 0;
            }
            else
            {
                *in_double_quote = 1;
                // Check for matching closing quote
                int temp_j = j + 1;
                while (str[temp_j] && str[temp_j] != '\"')
                    temp_j++;
                if (!str[temp_j])
                {
                    // No matching closing quote, keep this quote
                    str[(*k)++] = str[j];
                }
            }
        }
        else if (str[j] == '$'
            && *in_double_quote == 0 && *in_single_quote == 0 && str[j+1] != '\0' && !is_alpha(str[j+1]))
        {
            if (str[j] == '\'' && !*in_double_quote)
				*in_single_quote = !*in_single_quote;
			else if (str[j] == '\"' && !*in_single_quote)
				*in_double_quote = !*in_double_quote;
        }
        else
        {
            str[(*k)++] = str[j];
        }
        j++;
    }
}

void delete_quotes_from_string(char *str)
{
    int in_single_quote = 0;
    int in_double_quote = 0;
    int k = 0;
    delete_quotes_from_string_helper(str, &k, &in_single_quote, &in_double_quote);
    str[k] = '\0';
}


void	delete_quotes_from_files(t_minishell *minishell)
{
	t_minishell			*temp;
	t_file_redirection	*files;

	temp = minishell;
	if (temp != NULL)
	{
		while (temp)
		{
			files = temp->files;
			while (files)
			{
				delete_quotes_from_string(files->filename);
				files = files->next;
			}
			temp = temp->next;
		}
	}
}
int count_char(char *str, char c)
{
	int count;
	int i = 0;
	count = 0;
	while(str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return count;
}

void	delete_quotes_from_args(t_minishell *minishell)
{
	t_minishell	*temp;
	int			i;

	i = 0;
	temp = minishell;
	if (temp->args != NULL)
	{
		while (temp && temp->args != NULL)
		{
			i = 0;
			while (temp->args[i])
			{
				delete_quotes_from_string(temp->args[i]);
				i++;
			}
			if (temp->command)
			{
				free(temp->command);
				temp->command = NULL;
			}
			if (temp->args[0])
				temp->command = ft_strdup(temp->args[0]);
			temp = temp->next;
		}
	}
}

t_minishell	*delete_quotes(t_minishell *minishell)
{
	t_minishell			*temp;

	temp = minishell;
	delete_quotes_from_args(temp);
	temp = minishell;
	delete_quotes_from_files(temp);
	return (minishell);
}
