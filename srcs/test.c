/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/07 01:48:35 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <prompt.h>


int	main(void)
{
	char		*res;
	int			type;
	t_block		*head;
	t_prompt	prompt_params;

	if (!refresh_prompt_param(&prompt_params))
		return (1);
	res = NULL;
	type = -1;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	readline(build_prompt(prompt_params));
	parse_cmd(&head, rl_line_buffer);
	free(rl_prompt);
	printf("[%d] res: '%s'\n", type, res);
}
