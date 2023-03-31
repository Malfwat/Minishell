/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:39:23 by amouflet          #+#    #+#             */
/*   Updated: 2023/01/25 15:09:59 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	main(int ac, char **av, char **env)
{
	t_task	task;
	int		exit_value;

	if (ac != 5)
		return (0);
	task = init_t_stack(ac, av);
	get_cmd(ac - 2, av + 2, env, &task.cmd);
	x_pipe(task);
	exit_value = task.cmd->end->exit_value;
	f_err_get_cmd(task.cmd->begin);
	return (exit_value);
}
