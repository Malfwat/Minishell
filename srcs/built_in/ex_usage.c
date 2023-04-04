/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_usage.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:49:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 21:26:18 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <env_function.h>
#include <struct.h>

int	main(int ac, char **av, char **envp)
{
	t_env_var	*env_lst;

	if (ac != 2)
		return (0);
	env_lst = get_env_var(envp);
	pwd();
	cd(env_lst, av[1]);
	pwd();
	echo(0, 3, "tst", "ok", "non");
	free_env_lst(&env_lst);
	return (0);
}
