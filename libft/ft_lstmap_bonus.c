/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:17:50 by akrid             #+#    #+#             */
/*   Updated: 2023/11/17 15:18:55 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*maped;
	t_list	*temp;

	maped = NULL;
	if (lst && f && del)
	{
		temp = ft_lstnew(f(lst->content));
		if (temp == NULL)
			return (NULL);
		maped = temp;
		lst = (lst->next);
		while (lst)
		{
			temp = ft_lstnew(f(lst->content));
			if (!temp)
			{
				ft_lstclear(&maped, del);
				return (NULL);
			}
			ft_lstadd_back(&maped, temp);
			lst = (lst->next);
		}
	}
	return (maped);
}
