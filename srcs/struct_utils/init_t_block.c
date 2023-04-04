/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:02:38 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 19:15:59 by hateisse         ###   ########.fr       */
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
	// new->cmd_line = line;
	// remplir egalement t_cmds
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

int	add_block_back(t_block **head, /* char *line,  */t_block *(*last)(t_block *))
{
	char	*tmp;
	if (*head == NULL)
	{
		*head = new_block();
		if (*head == NULL)
			return (1); // ajouter une fonction print error
	}
	else
	{
		tmp = *head;
		last(tmp)->next = new_block();
	}
	return (0);
}
