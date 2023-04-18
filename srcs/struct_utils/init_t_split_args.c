/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_t_split_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 06:01:40 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 13:41:12 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <ms_define.h>
#include <libft.h>
#include <errno.h>
#include <stdlib.h>
#include <minishell.h>

void	free_t_split_arg(t_split_arg **arg)
{
	t_split_arg	*ptr;

	while (*arg)
	{
		ptr = (*arg)->next;
		free((*arg)->str);
		free(*arg);
		*arg = ptr;
	}
	*arg = NULL;
}

void	ls_split_args_addback(t_split_arg **head, t_split_arg *new)
{
	t_split_arg	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_split_arg	*ls_split_args_new(char *data, char scope)
{
	t_split_arg	*new;

	if (!data)
		return (NULL);
	new = ft_calloc(1, sizeof(t_split_arg));
	if (!new)
		return (NULL);
	new->str = data;
	new->scope = scope;
	return (new);
}

char	*join_splitted_arg(t_split_arg *arg, t_env_var *envp, bool interpret)
{
	char	*res;
	char	*tmp;
	char	*dollar_interpreted;

	res = NULL;
	dollar_interpreted = NULL;
	while (arg)
	{
		tmp = res;
		if (interpret)
		{
			dollar_interpreted = interpret_dollars(arg, envp);
			if (!dollar_interpreted && !res)
				dollar_interpreted = ft_strdup("");
			res = ft_strjoin(res, dollar_interpreted);
			free(dollar_interpreted);
		}
		else
			res = ft_strjoin(res, arg->str);
		free(tmp);
		if (errno)
			return (free(res), NULL);
		arg = arg->next;
	}
	return (res);
}