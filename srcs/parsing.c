/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:08:32 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/01 15:41:41 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct.h>
#include <minishell.h>
#include <libft.h>

t_block	*new_block(char *line)
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
	while (head->next)
		head = head->next;
	return (head);
}

t_block	*last_pipe(t_block *head)
{
	while (head->pipe_next)
		head = head->pipe_next;
	return (head);
}

t_block	*last_sub(t_block *head)
{
	while (head->sub)
		head = head->sub;
	return (head);
}


int	is_delimiter(char *str, int i)
{
	if (str[i] == '|')
	{
		if (str[i + 1] == '|')
			return (OR_OPERATOR);
		return (PIPE_OPERATOR);
	}
	if (str[i] == '&' && str[i + 1] == '&')
		return (AND_OPERATOR);
	if (str[i] == ';')
		return (SEMI_COLON);
	return (-1);
}

// function using substr
int trouve_la_prochaine_portion_a_etudier(char *cmd_line, char **str/* a remplir avec la portion retiree */, \
int *positon_dans_la_string)
{
	int i = *positon_dans_la_string;
	int delimiter;

	while (cmd_line[i])
	{
		delimiter = is_delimiter(cmd_line, i);
		if (delimiter != -1)
			break;
		i++;
	}
	*str = ft_substr(cmd_line, *positon_dans_la_string, i);
	return (delimiter);
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


int	add_block_back(t_block **head, char *line, t_block *(*last)(t_block *))
{
	char	*tmp;
	if (*head == NULL)
	{
		*head = new_block(line);
		if (*head == NULL)
			return (1); // ajouter une fonction print error
	}
	else
	{
		tmp = *head;
		last(tmp)->next = new_block(line);
	}
	return (0);
}


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
