/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/03 12:27:41 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct.h>
#include <libft.h>
#include <stdlib.h>

t_env_var	*new_env_var(char *str)
{
	t_env_var	*new;

	new = malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->var = str;
	new->next = NULL;
	return (new);
}

t_env_var	*get_last(t_env_var *tmp)
{
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	add_env_var(t_env_var **head, char *str)
{
	t_env_var	*new;

	new = new_env_var(str);
	if (!new)
		return (-1);
	if (!*head)
		*head = new;
	else 
		get_last(*head)->next = new;
	return (0);
}

void	free_env_lst(t_env_var **lst)
{
	t_env_var	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->var);
		free(*lst);
		*lst = tmp;
	}
}

t_env_var	*get_env_var(char **env)
{
	t_env_var	*lst;
	int	i;

	i = 0;
	lst = NULL;
	while (env && env[i])
	{
		if (add_env_var(&lst, env[i++]))
			return (free_env_lst(&lst), NULL);
	}
	return (lst);
}

char **t_env_var_to_array(t_env_var	*lst)
{
	char		**tab;
	int			len;
	t_env_var	*tmp;

	tmp = lst;
	while (lst)
	{
		lst = lst->next;
		len++;
	}
	tab = malloc(sizeof(char *) * (len + 1));
	if (!tab)
		return (NULL);
	tab[len] = NULL;
	len = 0;
	while (tmp)
	{
		tab[len] = tmp->var;
		len++;
		tmp = tmp->next;
	}
	return (tab);
}

void	print_tab(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], 1);
		i++;
	}
	return ;
}

int	main(int ac, char **av, char **env)
{
	t_env_var	*env_lst;

	(void)ac;
	(void)av;
	env_lst = get_env_var(env);
	print_tab(t_env_var_to_array(env_lst));
	return (0);
}