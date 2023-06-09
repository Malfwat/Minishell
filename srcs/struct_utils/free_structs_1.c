/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:25:21 by marvin            #+#    #+#             */
/*   Updated: 2023/05/05 06:13:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_struct.h>
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
	free(ms_params.prev_line);
	free(ms_params.previous_directory);
	free_prompt_params(&ms_params.prompt_params);
	free_env_lst(ms_params.envp);
	ms_params.envp = NULL;
}

void	free_env_lst(t_env *envp_lst)
{
	t_env	*tmp;

	while (envp_lst)
	{
		tmp = envp_lst->next;
		free(envp_lst->var_name);
		free(envp_lst->var_value);
		free(envp_lst);
		envp_lst = tmp;
	}
}
