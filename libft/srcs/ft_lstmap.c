/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 21:02:39 by malfwa            #+#    #+#             */
/*   Updated: 2022/11/15 14:55:28 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_all(t_list *begin, void (*del)(void *))
{
	t_list	*tmp;

	while (begin != NULL)
	{
		tmp = begin;
		begin = begin->next;
		del(tmp);
		free(tmp);
	}
	return (begin);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin;
	t_list	*new_lst;

	begin = NULL;
	new_lst = malloc(sizeof(t_list));
	if (new_lst == NULL)
		return (free_all(begin, del));
	new_lst->content = f(lst->content);
	new_lst->next = NULL;
	begin = new_lst;
	lst = lst->next;
	while (lst != NULL)
	{
		new_lst->next = malloc(sizeof(t_list));
		if (new_lst->next == NULL)
			return (free_all(begin, del));
		new_lst->next->content = f(lst->content);
		new_lst->next->next = NULL;
		new_lst = new_lst->next;
		lst = lst->next;
	}
	return (begin);
}
