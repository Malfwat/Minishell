/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 20:26:59 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/25 21:32:33 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <string.h>
#include <libft.h>
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
		ft_putstr_fd(subname, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error, 2);
}

void	handle_execve_failure(t_minishell ms_params, char *program_name)
{
	int	exit_value;

	exit_value = 2;
	if (errno != ENOENT)
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
	exit_ms(ms_params, exit_value, NULL);
}

void	exit_ms(t_minishell ms_params, int exitv, char *context)
{
	flood_free(ms_params.head);
	infanticides(ms_params.children);
	free_children(&ms_params.children);
	if ((ms_params.flags & C_FLAG) == 0)
	{
		restore_terminal_params(ms_params.saved_params, ms_params.stdin_fileno);
		clear_history();
		if (ms_params.history_fd)
			close(ms_params.history_fd);
		free(rl_line_buffer);
	}
	close(ms_params.stdin_fileno);
	free_ms_params(ms_params);
	if (errno)
		ms_perror("minishell7", context, strerror(errno));
	exit(exitv);
}
