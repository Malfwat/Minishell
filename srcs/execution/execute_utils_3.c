/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 01:03:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 01:12:30 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <ms_struct.h>
#include <libft.h>
#include <minishell.h>
#include <stdlib.h>
#include <ms_exec.h>

static char	*update_res(char *str, char *res)
{
	if (*str == '$' && (*str + 1) == '$')
		res = ft_strjoin(res, str + 1);
	else
		res = ft_strjoin(res, str);
	return (res);
}

static void	update_argv_tab(char ***tab, char *res, bool scope)
{
	if (scope)
	{
		*tab = ft_calloc(2, sizeof(char *));
		if (!*tab)
			return ;
		return ((void)((*tab)[0] = res));
	}
	return (*tab = ft_split(res, ' '), free(res));
}

bool	is_first_identifier_char_valid(char c)
{
	char	str[2];

	str[0] = c;
	str[1] = 0;
	if (ft_strchrnul_nm(str, ALPHA""DIGIT""UC"@!?#*$") == str)
		return (false);
	return (true);
}

void	put_in_list(t_args *prev, t_args *next, t_args *to_insert)
{
	prev->next = to_insert;
	to_insert->prev = prev;
	if (next)
		next->prev = last_args(to_insert);
	last_args(to_insert)->next = next;
}

void	interpret_dollars(t_s_arg *arg, t_env *envp, char ***new)
{
	char		*tmp;
	char		*str;
	char		*dollar_var;
	char		*res;

	res = NULL;
	str = arg->str;
	dollar_var = slice_next_part_to_interpret(&str);
	while (!errno && dollar_var && *dollar_var)
	{
		tmp = res;
		if ((*dollar_var != '$') || (arg->scope == '\'')
			|| !is_first_identifier_char_valid(*(dollar_var + 1)))
			res = update_res(dollar_var, res);
		else
			res = replace_dollars_var(res, envp, dollar_var + 1);
		free(dollar_var);
		free(tmp);
		dollar_var = slice_next_part_to_interpret(&str);
	}
	return (update_argv_tab(new, res, arg->scope));
}
