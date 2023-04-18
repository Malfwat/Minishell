/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:54:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 05:56:25 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <stdlib.h>
#include <errno.h>
#include <struct_ms.h>

void	wc_update_t_args(t_args **args)
{
	t_args	*lst;
	t_args	*tmp;
	t_args	*wildcard;

	lst = *args;
	wildcard = NULL;
	while (lst)
	{
		if (lst->final_arg && ft_strchr(lst->final_arg, '*'))
		{
			manage_wildcard(&wildcard, lst->final_arg);
			if (wildcard)
			{
				tmp = lst->next;
				insert_t_args(args, lst, wildcard);
				lst = tmp;
			}
			else
				lst = lst->next;
		}
		else
			lst = lst->next;
	}
}

char	*interpret_dollars(t_split_arg *arg, t_env_var *envp)
{
	char		*tmp;
	char		*res;
	t_env_var	*env_var;
	char		**tab;
	int			i;

	tab = ft_split(arg->str, '$');
	if (!tab)
		return (NULL);
	i = -1;
	res = NULL;
	while (tab[++i] && !errno)
	{
		tmp = res;
		if ((i == 0 && arg->str[0] != '$') || (arg->scope == '\''))
			res = ft_strjoin(res, tab[i]);
		else
		{
			env_var = find_env_var(envp, tab[i]);
			if (!env_var)
				res = ft_strjoin(res, "");
			else
				res = ft_strjoin(res, env_var->var_value);
		}
		free(tmp);
	}
	return (ft_strsfree(tab), res);
}
