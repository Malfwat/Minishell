/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:15:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 05:19:54 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env_function.h>
#include <stdlib.h>
#include <libft.h>
#include <stdio.h>

int	main(int ac, char **av, char **envp)
{
	t_env_var	*env_lst;
	char *str1 = "test=sdfsf";
	char *str3 = "test=sdfsf";
	char *str2 = "test2=bonjour";
	(void)ac;
	(void)av;

	env_lst = get_env_var(envp);
	env(env_lst);
	export(&env_lst, get_env_var_name(str1), get_env_var_value(str1), 0);
	export(&env_lst, get_env_var_name(str3), get_env_var_value(str3), 0);
	export(&env_lst, get_env_var_name(str2), get_env_var_value(str2), 1);
	update_env_var(&env_lst);
	env(env_lst);
	printf("\n\n\n\n");
	unset(&env_lst, find_env_var(env_lst, "test"));
	unset(&env_lst, find_env_var(env_lst, "test2"));
	env(env_lst);
	free_env_lst(&env_lst);
	return (0);
}