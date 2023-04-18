/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:33:48 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/18 13:26:36 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <libft.h>
#include <stdlib.h>
#include <minishell.h>
#include <stdio.h>
#include <errno.h>

t_args	*new_cmd_arg(t_split_arg *arg)
{
	t_args	*new;

	new = ft_calloc(1, sizeof(t_args));
	if (!new)
		return (NULL);
	new->s_args = arg;
	return (new);
}

t_args	*last_args(t_args *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	ft_ls_t_args_addback(t_args **head, t_split_arg *arg)
{
	t_args	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		new->prev = last_args(*head);
		new->prev->next = new;
	}
}

void	ft_addarg_front(t_args **head, t_split_arg *arg)
{
	t_args	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
	{
		new->next = *head;
		(*head)->prev = new;
		*head = new;
	}
}

void	insert_t_args(t_args **head, t_args *current, t_args *new_lst)
{
	if (*head == current)
		*head = new_lst;
	else
	{
		current->prev->next = new_lst;
		new_lst->prev = current->prev;
	}
	if (current->next)
		current->next->prev = last_args(new_lst);
	last_args(new_lst)->next = current->next;
	free(current->final_arg);
	free_t_split_arg(&current->s_args);
	free(current);
}
