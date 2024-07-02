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


void delete_quotes_from_string(char *str)
{
    if (str == NULL)
        return;

    int len = ft_strlen(str);
    int i = 0;
    int j = 0;

    while (i < len)
    {
        if ((str[i] == '"' || str[i] == '\'') && i + 1 < len)
        {
            char quote = str[i];
            int start = i;
            i++;
            while (i < len && str[i] != quote)
            {
                i++;
            }
            if (i < len && str[i] == quote)
            {
                // Found matching quote, skip both quotes
                i++;
                while (start + 1 < i - 1)
                {
                    str[j++] = str[++start];
                }
            }
            else
            {
                // No matching quote found, keep the opening quote
                str[j++] = str[start];
                i = start + 1;
            }
        }
        else
        {
            str[j++] = str[i++];
        }
    }
    str[j] = '\0';
}

// void delete_quotes_from_string(char *str)
// {
// 	int in_quote;
// 	char quote;
// 	int 	i;

// 	in_quote = 0;
// 	quote = '\0';
// 	i = 0;
// 	int index = 0;

// 	if (str == NULL)
// 		return ;	

// 	int len = ft_strlen(str);
// 	while(i < len)
// 	{
// 		printf("%s%c %s", RED, str[i] , RESET);
// 		// printf("\n==================\n");
// 		// printf("quote = [%c]\n" , quote);
// 		// printf("in_quote = %d\n", in_quote);
// 		// printf("\n==================\n");
// 		if (str[i] == '\"' && in_quote == 0 && (quote == '\0' || quote != '\''))
// 		{
// 			// if (delete_double_quotes(&i, str, &in_quote, &quote) == 1)
// 			// 	break;
// 			in_quote = 1;
// 			quote = '\"';
// 			index = i;
// 			i++;
// 			while(str[i] && str[i] != quote)
// 				i++;
// 			if(str[i] != '\0')
// 			{
// 				printf("index = %d\n", index);
// 				printf("index2 = %d\n", i );
// 				ft_remove_2_chars(str, index, i);
// 				in_quote = 0;
// 				quote = '\0';
// 				printf("quote = [%c]\n", quote);
// 				printf("%shere is the string after removing = [%s]%s\n", GREEN, str, RESET);
// 			}
// 			if (str[i] == '\0')
// 				break;
// 		}
// 		else if (str[i] == '\'' && (quote == '\0' || quote != '\"'))
// 		{
// 			in_quote = 1;
// 			quote = '\'';
// 			index = i;
// 			i++;
// 			while(str[i] && str[i] != quote)
// 				i++;
// 			if(str[i] != '\0')
// 			{
// 				printf("index = %d\n", index);
// 				printf("index2 = %d\n", i );
// 				ft_remove_2_chars(str, index, i);
// 				in_quote = 0;
// 				quote = '\0';
// 				printf("quote = [%c]\n", quote);
// 				printf("%shere is the string after removing = [%s]%s\n", GREEN, str, RESET);
// 			}
// 			if (str[i] == '\0')
// 				break;
// 		}
// 		printf("i = %d\n", i);
// 		i++;
// 	}
// 	printf("\n");
// }





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
			temp = temp->next;
		}
		if (minishell->command)
		{
			free(minishell->command);
			minishell->command = NULL;
		}
		if (minishell->args[0])
			minishell->command = ft_strdup(minishell->args[0]);
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
