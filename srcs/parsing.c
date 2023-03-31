/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/31 20:35:13 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct.h>
#include <libft.h>

t_block	*new_block(char *line)
{
	t_block	*new;

	new = ft_calloc(1, sizeof(t_block));
	if (!new)
		return (NULL);
	new->cmd_line = line;
	// remplir egalement t_cmds
	return (new);
}

t_block	*last_sibling(t_block *head)
{
	while (head->next)
		head = head->next;
	return (head);
}

int	find_closing_parenthesis(char *str)
{
	int counter;
	int i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			counter += 1;
		else if (str[i] == ')')
			counter -= 1;
		if (counter == 0)
			return (i - 1);
		i += 1;
	}
	return (-1);
}


int	add_block_back(t_block **head, char *line)
{
	char	*tmp;
	if (*head == NULL)
	{
		*head = new_block(line);
		if (*head == NULL)
			return (1); // ajouter print pour localiser l'erreur
	}
	else
	{
		tmp = *head;
		last->sibling(tmp)->next = new_block(line);
	}
	return (0);
}

((eede)  (dedede)) (ededede)



int main(int argc, char **argv, char **envp)
{
	t_block	*tmp;

	tmp = NULL;
	if (argc < 2)
		return (0);
	add_block_back(&tmp, line);
	add_block_back(&tmp, line2);
	add_block_back(&tmp->sub, line3);
}
