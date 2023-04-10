/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 20:26:59 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/10 16:51:47 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <string.h>
#include <libft.h>

void	exit_ms(t_minishell ms_params, int exitv, char *context)
{
	flood_free(ms_params.head);
	free_children(ms_params.children);
	restore_terminal_params(ms_params.saved_params, ms_params.stdin_fileno);
	free_ms_params(ms_params);
	close(ms_params.stdin_fileno);
	if (errno)
		ms_perror("minishell", context, strerror(errno));
	exit(exitv);
}