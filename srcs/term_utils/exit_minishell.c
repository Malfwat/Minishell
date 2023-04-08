/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_terminal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 04:45:01 by marvin            #+#    #+#             */
/*   Updated: 2023/04/08 04:45:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>

void	exit_minishell(t_block *lst, t_minishell *ms_params)
{
	flood_free(block);
	free_ms_params(ms_params);
	restore_terminal(ms_params->saved_params);
	exit(1);
}
