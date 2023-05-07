/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 14:11:14 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 14:23:24 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_parsing.h>
#include <ms_define.h>
#include <ms_exec.h>

bool	is_line_empty(char *u_in)
{
	if (!*(u_in + pass_whitespaces(u_in)))
	{
		set_env_exit_var(0);
		g_ms_params.last_exit_code = 0;
		return (true);
	}
	return (false);
}