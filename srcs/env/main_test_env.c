/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:15:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 21:26:51 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <env_function.h>
#include <stdlib.h>
#include <libft.h>
#include <stdio.h>

// int	main(int ac, char **av, char **envp)
// {
// 	t_env_var	*env_lst;
// 	char *str1 = ft_strdup("test=sdfsf");
// 	char *str3 = ft_strdup("test=sdfsf");
// 	char *str2 = ft_strdup("test2=bonjour");
// 	(void)ac;
// 	(void)av;

// 	env_lst = get_env_var(envp);
// 	env(env_lst);
// 	export(&env_lst, str1);
// 	export(&env_lst, str3);
// 	export(&env_lst, str2);
// 	env(env_lst);
// 	printf("\n\n\n\n");
// 	unset(&env_lst, "test");
// 	unset(&env_lst, "test2");
// 	env(env_lst);
// 	free_env_lst(&env_lst);
// 	return (0);
// }