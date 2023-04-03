/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/03 13:50:58 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <struct.h>
#include <libft.h>
#include <stdlib.h>

t_env_var	*new_env_var(char *str)
{
	t_env_var	*new;

	if (!str)
		return (NULL);
	new = malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->var = str;
	new->prev = NULL;
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
	t_env_var	*last;

	new = new_env_var(str);
	if (!new)
		return (free(str), -1);
	if (!*head)
		*head = new;
	else
	{
		last = get_last(*head);
		last->next = new;
		new->prev = last;
	}
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
		if (add_env_var(&lst, ft_strdup(env[i++])))
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
	len = 0;
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

char *get_env_var_name(char *str)
{
	int	i;

	i = 0;
	while(str[i] != '=')
		i++;
	return (ft_substr(str, 0, i));
}

char *get_env_var_value(char *str)
{
	int	i;

	i = 0;
	while(str[i] != '=')
		i++;
	return (ft_substr(str, i + 1, ft_strlen(&str[i + 1])));
}

t_env_var	*find_env_var(t_env_var	*lst, char *str)
{
	char	*tmp;
	
	while (lst)
	{
		tmp = get_env_var_name(lst->var);
		if (ft_strcmp(str, tmp) == 0)
		{
			free(tmp);
			return (lst);
		}
		free(tmp);
		lst = lst->next;
	}
	return (lst);
}

int export(t_env_var **lst, char *str)
{
	t_env_var	*tmp;
	char		*name;

	name = get_env_var_name(str);
	if (!name)
		return (-1);
	tmp = find_env_var(*lst, name);
	free(name);
	if (!tmp)
	{
		if (add_env_var(lst, str))
			return (free_env_lst(lst), -1);
	}
	else
	{
		free(tmp->var);
		tmp->var = str;
	}
	return (0);
}


int	unset(t_env_var **head, char *name)
{
	t_env_var	*to_pop;

	to_pop = find_env_var(*head, name);
	if (!to_pop)
		return (0);
	if (to_pop->prev)
		to_pop->prev->next = to_pop->next;
	if (to_pop->next)
		to_pop->next->prev = to_pop->prev;
	free(to_pop->var);
	free(to_pop);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_env_var	*env_lst;
	char *str1 = ft_strdup("test=sdfsf");
	char *str3 = ft_strdup("test=sdfsf");
	char *str2 = ft_strdup("test2=bonjour");
	char **tab;
	(void)ac;
	(void)av;
	
	env_lst = get_env_var(env);
	export(&env_lst, str1);
	export(&env_lst, str3);
	export(&env_lst, str2);
	tab = t_env_var_to_array(env_lst);
	print_tab(tab);
	free(tab);
	printf("\n\n\n\n");
	unset(&env_lst, "test");
	unset(&env_lst, "test2");
	tab = t_env_var_to_array(env_lst);
	print_tab(tab);
	free(tab);
	free_env_lst(&env_lst);
	return (0);
}