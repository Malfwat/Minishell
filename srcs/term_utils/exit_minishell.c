/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:45:01 by marvin            #+#    #+#             */
/*   Updated: 2023/04/08 21:41:19 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>

void	exit_minishell(t_block *lst, t_minishell ms_params, int exit_value)
{
	flood_free(lst);
	free_ms_params(ms_params);
	restore_terminal_params(ms_params.saved_params);
	exit(exit_value);
}
