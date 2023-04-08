/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/08 07:04:25 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <env_function.h>
#include <libft.h>

void	export(t_env_var **lst, char *name, char *value, bool temp)
{
	t_env_var	*tmp;

	tmp = find_env_var(*lst, name);
	if (!tmp)
	{
		if (!add_env_var(lst, name, value, temp))
			return (free_t_env(*lst));
	}
	else
	{
		free(name);
		free(tmp->var_value);
		tmp->var_value = value;
	}
}

void	unset(t_env_var **head, t_env_var *to_pop)
{
	if (!to_pop)
		return ;
	if (to_pop->prev)
		to_pop->prev->next = to_pop->next;
	if (to_pop->next)
		to_pop->next->prev = to_pop->prev;
	if (to_pop == *head)
		*head = to_pop->next;
	free(to_pop->var_name);
	free(to_pop->var_value);
	free(to_pop);
}

void	env(t_env_var *lst)
{
	char	**tab;

	tab = build_envp(lst);
	ft_print_array_str(tab);
	ft_strsfree(tab);
}
