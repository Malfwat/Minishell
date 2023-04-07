/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/07 04:51:08 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <prompt.h>

#include <signal_ms.h>
// #include <libft.h>
// #include <get_next_line.h>
// #include <fcntl.h>

#include <history.h>



int	main(void)
{
	char		*res;
	int			type;
	t_block		*head;
	t_prompt	prompt_params;

	set_sig_handler();
	if (!refresh_prompt_param(&prompt_params))
		return (1);
	res = NULL;
	type = -1;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	int fd = get_my_history();
	if (fd == -1)
		return (2);
	readline(build_prompt(prompt_params));
	my_add_history(rl_line_buffer, fd);
	parse_cmd(&head, rl_line_buffer);
	free(rl_prompt);
	close(fd);
	printf("[%d] res: '%s'\n", type, res);
}
