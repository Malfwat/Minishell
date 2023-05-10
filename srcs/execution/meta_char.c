/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:54:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/10 21:25:50 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <stdlib.h>
#include <errno.h>
#include <minishell.h>
#include <ms_env_function.h>
#include <libft.h>
#include <ms_struct.h>

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

char	*find_suffix(char *str)
{
	char	*suffix;

	if (*str && ft_strchr(DIGIT"@!?#*$", *str))
		suffix = str + 1;
	else
		suffix = ft_strchrnul_nm(str, ALPHA""DIGIT""UC);
	return (suffix);
}

char	*replace_dollars_var(char *res, t_env *envp, char *var)
{
	t_env	*env_var;
	char	*dollar_var;
	char	*suffix;

	suffix = find_suffix(var);
	dollar_var = ft_substr(var, 0, suffix - var);
	env_var = find_env_var(envp, dollar_var);
	if (!env_var)
		res = ft_strsjoin(3, res, "", suffix);
	else
	{
		res = ft_strsjoin(3, res, env_var->var_value, suffix);
	}
	free(dollar_var);
	return (res);
}

char	*interpret_dollars_syntax(t_s_arg *arg, t_env *envp)
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
