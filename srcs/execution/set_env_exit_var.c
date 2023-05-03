/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_exit_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 21:50:58 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/03 22:13:16 by hateisse         ###   ########.fr       */
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
