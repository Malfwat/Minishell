/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 11:42:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/03 13:15:11 by malfwa           ###   ########.fr       */
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
		return (free(str), -1);
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
	
	tmp = get_env_var_name(lst->var);
	while (lst && ft_strcmp(str, tmp))
	{
		free(tmp);
		tmp = get_env_var_name(lst->var);
		lst = lst->next;
	}
	free(tmp);
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


// int	unset(t_env_var **head, char *name)
// {
	
// }

int	main(int ac, char **av, char **env)
{
	t_env_var	*env_lst;
	char *str1 = ft_strdup("test=sdfsf");
	char *str2 = ft_strdup("test=bonjour");
	char *res;
	(void)ac;
	(void)av;
	
	env_lst = get_env_var(env);
	export(&env_lst, str1);
	res = get_env_var_value(find_env_var(env_lst, "test")->var);
	printf("'%s'\n", res);
	free(res);
	export(&env_lst, str2);
	res = get_env_var_value(find_env_var(env_lst, "test")->var);
	printf("'%s'\n", res);
	free(res);
	free_env_lst(&env_lst);
	return (0);
}