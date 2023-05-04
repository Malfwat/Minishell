/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_exit_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 21:50:58 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/05 00:12:16 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_define.h>
#include <struct_ms.h>
#include <env_function.h>
#include <libft.h>

void	set_env_exit_var(int value)
{
	free(find_env_var(g_ms_params.envp, "?")->var_value);
	find_env_var(g_ms_params.envp, "?")->var_value = ft_itoa(value);
}

void	change_env_var_value(char *name, char *value)
{
	t_env	*var;
	
	var = find_env_var(g_ms_params.envp, name);
	if (!var)
		return ;
	free(var->var_value);
	var->var_value = value;
}
