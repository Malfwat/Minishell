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
#include <libft.h>
#include <stdbool.h>
#include <stdlib.h>
#include <minishell.h>
#include <unistd.h>
#include <parsing.h>
#include <readline/history.h>
#include <readline/readline.h>

// char	*get_next_param(char *str, int *i, int *type)
// {
// 	int		i;
// 	char	*res;

// 	i = 0;
// 	res = NULL;
// 	if (check_parenthesis_param(str, i, &res, type) \
// 		|| check_io_param(str, i, &res, type) \
// 		|| check_word_param(str, i, &res, type))
// 	{
// 		if (res == NULL)
// 			return (perror("malloc failed"), NULL);
// 	}
// 	return (res);
// }

int	main(void)
{
	char	*res = NULL;
	int		i = 0;
	int		type = -1;
	
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	ft_putstr_fd("\033[1m", 1);
	ft_putstr_fd("\033[38;5;10m", 1);
	// ft_putstr_fd("\033[38;5;9m", 1);
	
	readline("➜  \033[38;5;189mparsing \033[0m✗  ");

	res = get_next_param(rl_line_buffer, &i, &type);
	free(rl_line_buffer);
	free(rl_prompt);
	printf("[%d] res: '%s'\n", type, res);
}
