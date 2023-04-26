/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:13:08 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/26 16:36:08 by malfwa           ###   ########.fr       */
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


void	interpret_dollars_test(t_split_arg *arg, t_env_var *envp, char ***new)
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
	if (arg->scope)
	{
		*new = ft_calloc(2, sizeof(char *));
		(*new)[0] = res;
	}
	else
	{
		*new = ft_split(res, ' ');
		free(res);
	}
	return (ft_strsfree(tab));
}


char	*join_splitted_arg_test(t_args **args, t_env_var *envp, bool interpret)
{
	char	**tab;
	char	*res;
	char	*tmp;
	t_args	*tmp2;
	t_args	*test;
	char	*dollar_interpreted;
	t_split_arg *arg = ((*args))->s_args;

	res = NULL;
	dollar_interpreted = NULL;
	tab = NULL;
	while (arg)
	{
		tmp = res;
		if (interpret)
		{
			interpret_dollars_test(arg, envp, &tab);
			// if (!dollar_interpreted/*  && !res */)
				// dollar_interpreted = ;
			// tab = ft_split(dollar_interpreted, ' ');
			// if (!tab)
				// break ;
			if (!res)
				res = "";
			res = ft_strjoin(res, tab[0]);
			if (tab[0] && tab[1])
			{
				((*args))->final_arg = res;
				test = array_to_t_args(tab + 1);
				tmp2 = last_args(test);
				put_in_list((*args), (*args)->next, test);
				(*args) = tmp2;
				res = tmp2->final_arg;
			}
			ft_strsfree(tab);
			tab = NULL;
			free(dollar_interpreted);
		}
		else
			res = ft_strjoin(res, arg->str);
		free(tmp);
		if (errno)
			return (ft_strsfree(tab), free(res), NULL);
		arg = arg->next;
	}
	(*args)->final_arg = res;
	return (res);
}

bool	rebuild_args(t_args **head, t_env_var *envp)
{
	t_args	*ptr;

	ptr = *head;
	while (ptr)
	{
		join_splitted_arg_test(&ptr, envp, true);
		// ptr->final_arg = join_splitted_arg(ptr->s_args, envp, true);
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
