/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:54:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 00:50:06 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <stdlib.h>
#include <errno.h>
#include <minishell.h>
#include <env_function.h>
#include <libft.h>
#include <struct_ms.h>

void	update_t_args(t_args **args)
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

char	*replace_dollars_var(char *res, t_env *envp, char *var)
{
	t_env	*env_var;
	char	*dollar_var;
	char	*tmp;

	tmp = ft_strchrnul_set(var, "\'*");
	dollar_var = ft_substr(var, 0, tmp - var);
	env_var = find_env_var(envp, dollar_var);
	if (!env_var)
		res = ft_strsjoin(3, res, "", tmp);
	else
	{
		res = ft_strsjoin(3, res, env_var->var_value, tmp);
	}
	free(dollar_var);
	return (res);
}

char	*interpret_dollars(t_s_arg *arg, t_env *envp)
{
	char		*tmp;
	char		*res;
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
		{
			if (!i && arg->str[0] != '$')
				res = ft_strjoin(res, tab[i]);
			else
				res = ft_strsjoin(3, res, "$", tab[i]);
		}
		else
			res = replace_dollars_var(res, envp, tab[i]);
		free(tmp);
	}
	return (ft_strsfree(tab), res);
}
