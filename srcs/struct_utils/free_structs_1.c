/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:25:21 by marvin            #+#    #+#             */
/*   Updated: 2023/04/08 21:34:16 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>

void	free_ms_params(t_minishell ms_params)
{
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
