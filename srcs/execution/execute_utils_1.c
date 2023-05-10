/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:13:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 01:11:24 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_exec.h>

char	*slice_next_part_to_interpret(char **str)
{
	char	*dollar_var;
	char	*end;

	if (!*str || !**str)
		return (NULL);
	end = ft_strchrnul(*str + 1, '$');
	if (**str == '$' && *end == '$' && end == *str + 1)
		end += 1;
	dollar_var = ft_substr(*str, 0, end - *str);
	*str = end;
	return (dollar_var);
}

static void	insert_splitted_dollars_arg(t_args **args, char **res, char **tab)
{
	t_args		*end_of_lst;
	t_args		*lst;

	((*args))->final_arg = *res;
	lst = array_to_t_args(tab + 1);
	end_of_lst = last_args(lst);
	put_in_list((*args), (*args)->next, lst);
	(*args) = end_of_lst;
	*res = end_of_lst->final_arg;
}

void	join_splitted_arg(t_args **args, t_env *env, t_s_arg *arg, bool ch)
{
	char	**tab;
	char	*res;
	char	*tmp;

	res = NULL;
	while (arg)
	{
		tmp = res;
		if (ch)
		{
			interpret_dollars(arg, env, &tab);
			res = ft_strjoin((char *[]){"", res}[(res != NULL)], tab[0]);
			if (tab[0] && tab[1])
				insert_splitted_dollars_arg(args, &res, tab);
			ft_strsfree(tab);
		}
		else
			res = ft_strjoin(res, arg->str);
		free(tmp);
		if (errno)
			return (free(tab), free(res));
		arg = arg->next;
	}
	(*args)->final_arg = res;
}
