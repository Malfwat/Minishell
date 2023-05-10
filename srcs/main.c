/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/11 00:25:43 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <ms_parsing.h>

t_minishell	g_ms_params;

int	main(int ac, char **av, char **envp)
{
	if (!init_minishell(&g_ms_params, ac, av, envp))
		return (1);
	while (1)
		if (init_and_parse_input(av))
			handle_parsed_user_input();
	return (1);
}
