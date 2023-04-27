/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:13:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 03:05:15 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>

void	close_sub_fds(t_block *head)
{
	while (head)
	{
		if (head->io_tab[1] != INIT_FD_VALUE)
			close(head->io_tab[1]);
		if (head->io_tab[0] != INIT_FD_VALUE)
			close(head->io_tab[0]);
		head = head->next;
	}
}

void	free_exec_vars(t_exec_vars exec_vars)
{
	free(exec_vars.argv);
	ft_strsfree(exec_vars.envp);
	ft_strsfree(exec_vars.path);
}

t_args	*array_to_t_args(char **tab)
{
	int		size;
	t_args	*new;

	new = NULL;
	size = 0;
	while (tab[size])
	{
		ft_ls_t_args_addback(&new, NULL);
		if (errno)
			return (free_t_args(new), new);
		last_args(new)->final_arg = ft_strdup(tab[size++]);
	}
	return (new);
}

void	put_in_list(t_args *prev, t_args *next, t_args *to_insert)
{
	prev->next = to_insert;
	to_insert->prev = prev;
	if (next)
		next->prev = last_args(to_insert);
	last_args(to_insert)->next = next;
}

static char	*update_res(char c, char *str, char *res, int i)
{
	if (i == 0 && c != '$')
		res = ft_strjoin(res, str);
	else
		res = ft_strsjoin(3, res, "$", str);
	return (res);
}

void	interpret_dollars_test(t_s_arg *arg, t_env *envp, char ***new)
{
	char		*tmp;
	char		*res;
	char		**tab;
	int			i;

	tab = ft_split(arg->str, '$');
	if (!tab)
		return ;
	i = -1;
	res = NULL;
	while (tab[++i] && !errno)
	{
		tmp = res;
		if ((i == 0 && arg->str[0] != '$') || (arg->scope == '\''))
			res = update_res(arg->str[0], tab[i], res, i);
		else
			res = replace_dollars_var(res, envp, tab[i]);
		free(tmp);
	}
	if (arg->scope)
	{
		*new = ft_calloc(2, sizeof(char *));
		return ((*new)[0] = res, ft_strsfree(tab));
	}
	return (*new = ft_split(res, ' '), free(res), ft_strsfree(tab));
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

void	join_splitted_arg_test(t_args **args, t_env *env, t_s_arg *arg, bool ch)
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
			interpret_dollars_test(arg, env, &tab);
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

bool	rebuild_args(t_args **head, t_env *envp)
{
	t_args	*ptr;

	ptr = *head;
	while (ptr)
	{
		join_splitted_arg_test(&ptr, envp, ptr->s_args, true);
		if (errno)
			return (false);
		ptr = ptr->next;
	}
	return (true);
}

t_block	*find_next_block(t_block *block, bool ignore_sub)
{
	if (block->sub && !ignore_sub)
		return (block->sub);
	else if (block->pipe_next)
		return (block->pipe_next);
	else if (block->next)
		return (block->next);
	return (NULL);
}
