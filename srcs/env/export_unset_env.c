/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:06:09 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 21:26:34 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <env_function.h>
#include <libft.h>

int	export(t_env_var **lst, char *str)
{
	t_env_var	*tmp;
	char		*name;

	name = get_env_var_name(str);
	if (!name)
		return (-1);
	tmp = find_env_var(*lst, name);
	free(name);
	if (!tmp)
	{
		if (!add_env_var(lst, str))
			return (free_env_lst(lst), -1);
	}
	else
	{
		free(tmp->var);
		tmp->var = str;
	}
	return (0);
}

int	unset(t_env_var **head, char *name)
{
	t_env_var	*to_pop;

	to_pop = find_env_var(*head, name);
	if (!to_pop)
		return (0);
	if (to_pop->prev)
		to_pop->prev->next = to_pop->next;
	if (to_pop->next)
		to_pop->next->prev = to_pop->prev;
	if (to_pop == *head)
		*head = to_pop->next;
	free(to_pop->var);
	free(to_pop);
	return (0);
}

void	env(t_env_var *lst)
{
	char	**tab;

	tab = t_env_var_to_array(lst);
	ft_print_array_str(tab);
	free(tab);
}
