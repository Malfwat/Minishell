/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 14:27:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_define.h>
#include <ms_signal.h>
#include <ms_parsing.h>
#include <ms_prompt.h>
#include <ms_env_function.h>
#include <libft.h>
#include <sys/stat.h>
#include <ms_history.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ms_exec.h>
#include <ncurses.h>
#include <term.h>

t_minishell	g_ms_params;

int	main(int ac, char **av, char **envp)
{
	char	*user_input;

	if (!init_minishell(&g_ms_params, ac, av, envp))
		return (1);
	user_input = NULL;
	while (1)
	{
		if (!init_and_parse_input(av))
			continue ;
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
	return (1);
}
