/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_usage.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:49:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:20:38 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_env_function.h>
#include <ms_struct.h>

void	ms_echo(bool nl, int nb, ...);

int	main(int ac, char **av, char **envp)
{
	t_env	*env_lst;

	if (ac != 2)
		return (0);
	env_lst = get_env(envp);
	pwd();
	cd(env_lst, av[1]);
	pwd();
	ms_echo(0, 3, "tst", "ok", "non");
	free_env_lst(env_lst);
	return (0);
}
