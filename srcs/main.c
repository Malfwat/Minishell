/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/28 07:16:37 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_define.h>
#include <signal_ms.h>
#include <parsing_ms.h>
#include <prompt.h>
#include <env_function.h>
#include <libft.h>
#include <sys/stat.h>
#include <history.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <exec_ms.h>
#include <ncurses.h>
#include <term.h>

t_minishell	g_ms_params;

void	handler_heredoc(int num)
{
	(void)num;
	if (g_ms_params.heredoc_pid > 0)
		kill(g_ms_params.heredoc_pid, SIGKILL);
}

// void	handler_func(int num)
// {
// 	char		*ms_prompt_up;
// 	int			last_exit_code;

// 	if (g_ms_params.children)
// 		return ;
// 	last_exit_code = 130;
// 	errno = 0;
// 	(void)num;
// 	if (!refresh_prompt_param(&g_ms_params.prompt_params, last_exit_code))
// 		exit_ms(g_ms_params, 0, "promp2t");
// 	if (g_ms_params.heredoc_pid)
// 		kill(g_ms_params.heredoc_pid, SIGTERM);
// 	write(1, "\n", 1);
// 	ms_prompt_up = build_prompt(&g_ms_params.prompt_params, P_HEADER);
// 	ft_putstr_fd(ms_prompt_up, 1);
// 	free(ms_prompt_up);
// 	free_prompt_params(&g_ms_params.prompt_params);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

bool	is_line_empty(t_minishell *ms_params, char *u_in)
{
	if (!*(u_in + pass_whitespaces(u_in)))
	{
		free(find_env_var(ms_params->envp, "?")->var_value);
		find_env_var(ms_params->envp, "?")->var_value = ft_strdup("0");
		ms_params->last_exit_code = 0;
		return (true);
	}
	return (false);
}

void	flag_case(t_minishell *ms_params, char **av)
{
	char	*u_in;

	av[2] += pass_whitespaces(av[2]);
	u_in = ft_strdup(av[2]);
	if (!u_in || !parse_user_input(ms_params, u_in))
		exit_ms(*ms_params, \
		extract_exit_code(ms_params->last_exit_code), \
		"parse (-c flag case)");
	execute_commands(g_ms_params.head, &g_ms_params);
	if (wait_children(&g_ms_params) == -1)
		exit_ms(g_ms_params, 2, "waitpid");
	free_children(&g_ms_params.children);
	flood_free(g_ms_params.head);
	g_ms_params.head = (t_block *){0};
	if (g_ms_params.flags & C_FLAG)
		exit_ms(g_ms_params, \
		extract_exit_code(g_ms_params.last_exit_code), "main_exit(-c)");	
}

bool	is_valid_cmd_line(t_minishell *ms_params, char *u_in)
{
	if (!u_in)
	{
		ft_putendl_fd("exit", 1);
		exit_ms(*ms_params, 0, "readline");
	}
	if (is_line_empty(ms_params, u_in))
		return (free(u_in), false);
	ms_add_history(u_in, ms_params);
	if (!parse_user_input(ms_params, u_in))
		return (false);
	return (true);
}

bool	init_and_parse_input(t_minishell *ms_params, char **av, t_fd *u_in_fd)
{

	if (ms_params->flags & C_FLAG)
		return (flag_case(ms_params, av), true);
	else
	{
		*u_in_fd = init_prompt(ms_params);
		if (*u_in_fd < 3)
			return (false);	
	}
	return (true);
}

int	main(int ac, char **av, char **envp)
{
	char	*user_input;
	t_fd	input_fd;

	if (!init_minishell(&g_ms_params, ac, av, envp))
		return (1);
	user_input = NULL;
	while (1)
	{
		if (!init_and_parse_input(&g_ms_params, av, &input_fd))
			continue ;
		get_next_line(input_fd, &user_input);
		while (user_input)
		{
			if (is_valid_cmd_line(&g_ms_params, user_input))
			{
				execute_commands(g_ms_params.head, &g_ms_params);
				if (wait_children(&g_ms_params) == -1)
					exit_ms(g_ms_params, 2, "waitpid");
				free_children(&g_ms_params.children);
				flood_free(g_ms_params.head);
				g_ms_params.head = (t_block *){0};
				free(user_input);
			}
			get_next_line(input_fd, &user_input);
		}
	}
	return (1);
}
