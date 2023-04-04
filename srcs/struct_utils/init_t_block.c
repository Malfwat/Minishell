/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:02:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 21:36:05 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <stdlib.h>

t_block	*new_block(void)
{
	t_block	*new;

	new = ft_calloc(1, sizeof(t_block));
	if (!new)
		return (NULL);
	return (new);
}

t_block	*last_sibling(t_block *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

t_block	*last_pipe(t_block *head)
{
	while (head && head->pipe_next)
		head = head->pipe_next;
	return (head);
}

t_block	*last_sub(t_block *head)
{
	while (head && head->sub)
		head = head->sub;
	return (head);
}

void	add_block_back(t_block **head, t_block *(*last)(t_block *))
{
	char	*tmp;

	if (*head == NULL)
		*head = new_block();
	else
	{
		tmp = *head;
		last(tmp)->next = new_block();
	}
}
