/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/24 17:49:25 by hateisse         ###   ########.fr       */
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

bool	parse_user_input(t_minishell *ms_params, char *user_input)
{
	t_block		*head;

	if (!*user_input)
	{
		ms_params->last_exit_code = 0;
		return (false);
	}
	head = new_block();
	if (parse_cmds(&head, user_input) == false)
	{
		ms_params->last_exit_code = 640; // exit status = 2
		return (flood_free(head), false);
	}
	if (errno)
		exit_ms(*ms_params, 2, "parsing");
	hd_manager(head);
	if (errno)
		exit_ms(*ms_params, 2, "io_manager");
	ms_params->head = head;
	return (true);
}

void	handler_func(int num)
{
	char		*ms_prompt_up;
	int			last_exit_code;

	if (g_ms_params.children)
		return ;
	last_exit_code = 130;
	errno = 0;
	(void)num;
	if (!refresh_prompt_param(&g_ms_params.prompt_params, last_exit_code))
		exit_ms(g_ms_params, 0, "promp2t");
	write(1, "\n", 1);
	ms_prompt_up = build_prompt(&g_ms_params.prompt_params, P_HEADER);
	ft_putstr_fd(ms_prompt_up, 1);
	free(ms_prompt_up);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

bool	init_and_parse_input(t_minishell *ms_params, char **av, char **u_in)
{
	if (ms_params->flags & C_FLAG)
	{
		av[2] += pass_whitespaces(av[2]);
		*u_in = ft_strdup(av[2]);
		if (!*u_in || !parse_user_input(ms_params, *u_in))
			exit_ms(*ms_params, extract_exit_code(ms_params->last_exit_code), "parse (-c flag case)");
	}
	else
	{
		init_prompt(ms_params, u_in);
		if (!*u_in)
		{
			ft_putendl_fd("exit", 1);
			exit_ms(*ms_params, 0, "readline");
		}
		ms_add_history(*u_in, ms_params);
		*u_in += pass_whitespaces(*u_in);
		if (!parse_user_input(ms_params, *u_in))
			return (false);
	}
	return (true);
}

int	main(int ac, char **av, char **envp)
{
	char	*user_input;

	if (!init_minishell(&g_ms_params, ac, av, envp))
		return (1);
	user_input = NULL;
	while (1)
	{
		if (!init_and_parse_input(&g_ms_params, av, &user_input))
			continue ;
		// init_prompt(&g_ms_params, &user_input);
		// if (!user_input)
		// {
		// 	ft_putendl_fd("exit", 1);
		// 	exit_ms(g_ms_params, 0, "readline");
		// }
		// ms_add_history(user_input, &g_ms_params);
		// if (!parse_user_input(&g_ms_params, user_input))
		// 	continue ;
		execute_commands(g_ms_params.head, &g_ms_params);
		if (wait_children(&g_ms_params) == -1)
			exit_ms(g_ms_params, 2, "waitpid");
		free_children(&g_ms_params.children);
		flood_free(g_ms_params.head);
		g_ms_params.head = (t_block *){0};
		if (g_ms_params.flags & C_FLAG)
			exit_ms(g_ms_params, extract_exit_code(g_ms_params.last_exit_code), "main_exit(-c)");
	}
	return (1);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_env_var	*env_lst;

// 	if (ac < 2)
// 		return (0);
// 	env_lst = get_env_var(envp);
// 	pwd();
// 	cd(NULL, env_lst, &av[1]);
// 	pwd();
// 	ms_echo(av + 2);
// 	// env(env_lst);
// 	// unset(&env_lst, find_env_var(env_lst, "TERM"));
// 	// export(&env_lst, "test=95", 0);
// 	// write(1, "\n\n", 2);
// 	// env(env_lst);
// 	free_env_lst(env_lst);
// 	return (0);
// }
