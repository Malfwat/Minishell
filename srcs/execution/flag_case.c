/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_case.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 14:25:13 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 22:31:53 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_exec.h>
#include <ms_parsing.h>
#include <minishell.h>

void	flag_case(char **av)
{
	char	*u_in;

	av[2] += pass_whitespaces(av[2]);
	u_in = ft_strdup(av[2]);
	if (!u_in || !parse_user_input(u_in))
		exit_ms(extract_exit_code(g_ms_params.last_exit_code), \
		"parse (-c flag case)");
	execute_commands(g_ms_params.head);
	if (wait_children() == -1)
		exit_ms(2, "waitpid");
	free_children(&g_ms_params.children);
	flood_free(g_ms_params.head);
	g_ms_params.head = (t_block *){0};
	exit_ms(extract_exit_code(g_ms_params.last_exit_code), "main_exit(-c)");
}
