/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:25:21 by marvin            #+#    #+#             */
/*   Updated: 2023/04/12 17:05:42 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <libft.h>

void	free_prompt_params(t_prompt *lst)
{
	free(lst->time);
	free(lst->cwd);
	free(lst->git_branch_name);
	ft_bzero(lst, sizeof(t_prompt));
}

void	free_ms_params(t_minishell ms_params)
{
	free_prompt_params(&ms_params.prompt_params);
	free_env_lst(ms_params.envp);
	ms_params.envp = NULL;
}

void	free_env_lst(t_env_var *envp_lst)
{
	t_env_var	*tmp;

	while (envp_lst)
	{
		tmp = envp_lst->next;
		free(envp_lst->var_name);
		free(envp_lst->var_value);
		free(envp_lst);
		envp_lst = tmp;
	}
}
