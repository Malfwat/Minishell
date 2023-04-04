/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_io_params.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:47:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 21:04:05 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <struct_ms.h>
#include <stdlib.h>

t_redirect	*new_cmd_arg(char *arg)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	if (ft_strncmp(arg, "<<", 2))
		new->heredoc = ft_memmove(arg, arg + 2, ft_strlen(arg) - 2);
	else if (arg[0] == '<')
		new->file_name = ft_memmove(arg, arg + 1, ft_strlen(arg) - 1);
	else if (ft_strncmp(arg, ">>", 2))
	{
		new->append = true;
		new->file_name = ft_memmove(arg, arg + 2, ft_strlen(arg) - 2);
	}
	else
		new->file_name = ft_memmove(arg, arg + 1, ft_strlen(arg) - 1);
	return (NULL);
}

t_redirect	*last_redirect(t_redirect *head)
{
	
	while (head && head->next)
		head = head->next;
	return (head);
}


void	ft_addargs(t_redirect **head, char *arg)
{
	t_redirect	*new;

	new = new_cmd_arg(arg);
	if (!new)
		return ;
	if (*head == NULL)
		*head = new;
	else
		last_redirect(*head)->next = new;
}

void	ft_add_io(t_block *block, char *io)
{
	if (io[0] == '<')
		ft_addargs(&block->input_redirect, io);
	else
		ft_addargs(&block->output_redirect, io);
}