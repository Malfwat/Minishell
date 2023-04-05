/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/04 17:46:59 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

int	main(void)
{
	char	*res = NULL;
	int		i = 0;
	int		type = -1;
	
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	printf("\033[1m");
	printf("\033[38;5;10m");
	// ft_putstr_fd("\033[38;5;9m", 1);
	
	readline("➜  \033[38;5;189mparsing \033[0m✗  ");

	//res = get_next_param(rl_line_buffer, &i, &type);
	free(rl_line_buffer);
	free(rl_prompt);
	printf("[%d] res: '%s'\n", type, res);
}