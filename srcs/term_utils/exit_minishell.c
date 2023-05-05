/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 20:26:59 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/05 00:58:51 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <string.h>
#include <libft.h>
#include <ms_define.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ncurses.h>

void	ms_perror(char *program, char *subname, char *error)
{
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	if (subname)
	{
		while (*subname)
		{
			if (*subname == '\n')
				ft_putstr_fd("\\n", 2);
			else
				ft_putchar_fd(*subname, 2);
			subname++;
		}
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error, 2);
}

void	handle_execve_failure(char *program_name)
{
	int	exit_value;

	exit_value = 2;
	if (errno != ENOENT && errno)
	{
		if (errno == EACCES)
			exit_value = 126;
		else if (errno)
			exit_value = 2;
		ms_perror("minishell6", program_name, strerror(errno));
	}
	else
	{
		ms_perror("minishell5", program_name, "Command not found");
		exit_value = 127;
	}
	errno = 0;
	exit_ms(exit_value, NULL);
}

void	exit_ms(int exitv, char *context)
{
	flood_free(g_ms_params.head);
	infanticides(g_ms_params.children);
	free_children(&g_ms_params.children);
	if ((g_ms_params.flags & C_FLAG) == 0)
	{
		restore_terminal_params(g_ms_params.saved_params, g_ms_params.stdin_fileno);
		clear_history();
		if (g_ms_params.history_fd > 2)
			close(g_ms_params.history_fd);
		free(rl_line_buffer);
	}
	my_close(g_ms_params.stdin_fileno, -2);
	free_ms_params(g_ms_params);
	if (errno)
		ms_perror("minishell7", context, strerror(errno));
	exit(exitv);
}
