/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:13:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/10 21:50:08 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_exec.h>

void	put_in_list(t_args *prev, t_args *next, t_args *to_insert)
{
	prev->next = to_insert;
	to_insert->prev = prev;
	if (next)
		next->prev = last_args(to_insert);
	last_args(to_insert)->next = next;
}

static char	*update_res(char *str, char *res)
{
	if (*str == '$' && (*str + 1) == '$')
		res = ft_strjoin(res, str + 1);
	else
		res = ft_strjoin(res, str);
	return (res);
}

bool	is_first_identifier_char_valid(char c)
{
	char str[2];

	str[0] = c;
	str[1] = 0;
	if (ft_strchrnul_nm(str, ALPHA""DIGIT""UC"@!?#*$") == str)
		return (false);
	return (true);
}


static char	*slice_next_part_to_interpret(char **str)
{
	char	*dollar_var;
	char	*end;

	if (!*str || !**str)
		return (NULL);
	end = ft_strchrnul(*str + 1 , '$');
	if (**str == '$' && *end == '$' && end == *str + 1)
		end += 1;
	dollar_var = ft_substr(*str, 0, end - *str);
	*str = end;
	return (dollar_var);
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
	if (arg->scope)
	{
		*new = ft_calloc(2, sizeof(char *));
		return ((void)((*new)[0] = res));
	}
	return (*new = ft_split(res, ' '), free(res));
}

// void	interpret_dollars(t_s_arg *arg, t_env *envp, char ***new)
// {
// 	char		*tmp;
// 	char		*res;
// 	char		**tab;
// 	int			i;

// 	tab = ft_split(arg->str, '$');
// 	if (!tab)
// 		return ;
// 	else if (!*tab)
// 		return (*new = ft_split(arg->str, ' '), ft_strsfree(tab));
// 	i = -1;
// 	res = NULL;
// 	while (tab[++i] && !errno)
// 	{
// 		tmp = res;
// 		if ((i == 0 && arg->str[0] != '$') || (arg->scope == '\'')
// 			|| !is_first_identifier_char_valid(tab[i][0]))
// 			res = update_res(arg->str[0], tab[i], res, i);
// 		else
// 			res = replace_dollars_var(res, envp, tab[i]);
// 		free(tmp);
// 	}
// 	if (arg->scope)
// 	{
// 		*new = ft_calloc(2, sizeof(char *));
// 		return ((*new)[0] = res, ft_strsfree(tab));
// 	}
	
// 	return (*new = ft_split(res, ' '), free(res), ft_strsfree(tab));
// }

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
