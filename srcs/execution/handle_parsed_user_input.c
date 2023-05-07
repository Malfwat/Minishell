/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parsed_user_input.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 23:44:40 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 23:49:30 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_parsing.h>
#include <ms_exec.h>
#include <unistd.h>

void	handle_parsed_user_input(void)
{
	char	*user_input;

	user_input = NULL;
	ms_gnl(g_ms_params.input_fd, &user_input, false);
	while (1)
	{
		if (is_valid_cmd_line(user_input))
		{
			execute_commands(g_ms_params.head);
			if (wait_children() == -1)
				exit_ms(2, "waitpid");
			free_children(&g_ms_params.children);
		}
		flood_free(g_ms_params.head);
		g_ms_params.head = (t_block *){0};
		ms_gnl(g_ms_params.input_fd, &user_input, false);
		if (!user_input)
			break ;
	}
	close(g_ms_params.input_fd);
	if (errno)
		exit_ms(2, "");
}
