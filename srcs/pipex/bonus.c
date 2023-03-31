/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 00:38:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/31 17:11:11 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	main(int ac, char **av, char **env)
{
	t_task	task;
	int		exit_value;

	if (ac < 5)
		return (0);
	task = init_t_stack(ac, av);
	if (ac == 6 && !ft_strncmp(av[1], "here_doc", 8))
	{
		if (get_cmd(ac - 3, av + 3, env, &task.cmd))
			return (0);
		task.append = 1;
		task.limiter = av[2];
	}
	else if (get_cmd(ac - 2, av + 2, env, &task.cmd))
		return (0);
	x_pipe(task);
	exit_value = task.cmd->end->exit_value;
	f_err_get_cmd(task.cmd->begin);
	return (exit_value);
}
