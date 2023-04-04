/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 21:04:01 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdlib.h>

t_arg	*new_cmd_arg(char *arg)
{
	t_arg	*new;

	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->name = arg;
	new->next = NULL;
	return (NULL);
}

t_arg	*last_arg(t_arg *head)
{
	
	while (head && head->next)
		head = head->next;
	return (head);
}


void	ft_addargs(t_arg **head, char *arg)
{
	t_arg	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
		last_arg(*head)->next = new;
}