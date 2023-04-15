/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 20:26:59 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/13 12:39:50 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <struct_ms.h>
#include <minishell.h>
#include <stdlib.h>
#include <string.h>
#include <libft.h>
#include <readline/readline.h>
#include <readline/history.h>

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

void handle_execve_failure(t_minishell ms_params, char *program_name)
{
	int exit_value;

	exit_value = 2;
	if (errno != ENOENT)
	{
		if (errno == EACCES)
			exit_value = 126; // 126 == command found but cannot be executed
		else if (errno)
			exit_value = 2; // valeur fourre-tout
		ms_perror("minishell", program_name, strerror(errno));
	}
	else
	{
		ms_perror("minishell", program_name, "Command not found");
		exit_value = 127; // 127 == command not found
	}
	errno = 0;
	exit_ms(ms_params, exit_value, NULL);
	// exit_ms(ms_params.envp, ms_params, exit_value);
}

void	exit_ms(t_minishell ms_params, int exitv, char *context)
{
	flood_free(ms_params.head);
	infanticides(ms_params.children);
	free_children(&ms_params.children);
	restore_terminal_params(ms_params.saved_params, ms_params.stdin_fileno);
	free_ms_params(ms_params);
	close(ms_params.stdin_fileno);
	free(rl_line_buffer);
	clear_history();
	if (ms_params.history_fd)
		close(ms_params.history_fd);
	if (errno)
		ms_perror("minishell", context, strerror(errno));
	exit(exitv);
}
