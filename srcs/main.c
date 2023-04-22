/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/22 02:35:30 by malfwa           ###   ########.fr       */
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
		ms_params->last_exit_code = 1;
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

	last_exit_code = 130;
	errno = 0;
	(void)num;
	if (!refresh_prompt_param(&g_ms_params.prompt_params, last_exit_code))
		exit_ms(g_ms_params, 0, "promp2t");
	write(1, "\n", 1);
	ms_prompt_up = build_prompt(&g_ms_params.prompt_params, UP);
	ft_putstr_fd(ms_prompt_up, 1);
	free(ms_prompt_up);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int ac, char **av, char **envp)
{
	char	*user_input;

	if (ac != 1)
		return (ft_putstr_fd("Usage:\t./minishell\n", 2), 1);
	if (!init_minishell(&g_ms_params, envp))
		return ((void)ac, (void)av, 1);
	user_input = NULL;
	while (1)
	{
		init_prompt(&g_ms_params, &user_input);
		if (!user_input)
			exit_ms(g_ms_params, 0, "readline");
		ms_add_history(user_input, &g_ms_params);
		if (!parse_user_input(&g_ms_params, user_input))
			continue ;
		execute_commands(g_ms_params.head, &g_ms_params);
		if (wait_children(&g_ms_params) == -1)
			exit_ms(g_ms_params, 2, "waitpid");
		free_children(&g_ms_params.children);
		flood_free(g_ms_params.head);
		g_ms_params.head = (t_block *){0};
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
