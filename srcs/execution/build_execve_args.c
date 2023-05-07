/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_execve_args.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 22:05:31 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 22:29:25 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <ms_exec.h>
#include <ms_parsing.h>

char	**build_argv(t_args **head, char **path, t_env *envp)
{
	char	**tab;
	int		i;
	t_args	*tmp;

	i = 0;
	if (errno)
		return (NULL);
	rebuild_args(head, envp);
	update_t_args(head);
	check_for_color(head);
	get_cmd_path(path, &(*head)->final_arg, &(*head)->cmd_w_path);
	tab = ft_calloc(t_arg_lst_len(*head) + 1, sizeof(char *));
	if (!tab)
		return (0);
	i = 0;
	tmp = *head;
	while (tmp)
	{
		tab[i++] = tmp->final_arg;
		tmp = tmp->next;
	}
	return (tab);
}

static int	t_env_lstlen(t_env	*envp, char scope)
{
	int	len;

	len = 0;
	while (envp)
	{
		if (envp->env_scope == scope || scope == ANY_ENV_SCOPE)
			len++;
		envp = envp->next;
	}
	return (len);
}

char	**build_envp(t_env	*envp)
{
	char	**tab;
	int		len;

	len = 0;
	len = t_env_lstlen(envp, ANY_ENV_SCOPE);
	tab = ft_calloc((len + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	len = 0;
	while (envp)
	{
		if (envp->env_scope == PUBLIC_VAR)
		{
			tab[len] = ft_strsjoin(3, envp->var_name, "=", envp->var_value);
			if (!tab[len++])
				return (ft_strsfree(tab), NULL);
		}
		envp = envp->next;
	}
	return (tab);
}
