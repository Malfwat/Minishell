/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 14:18:20 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 14:20:01 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_readline.h>
#include <minishell.h>

void	ms_gnl(t_fd fd, char **user_input, bool conserve_nl)
{
	char	*following_part;
	int		len;
	char	*tmp;
	char	quotes;

	get_next_line(fd, user_input);
	len = 0;
	if (*user_input)
		len = ft_strlen(*user_input);
	quotes = 0;
	update_quotes(*user_input, &quotes);
	while (*user_input && quotes)
	{
		len = ft_strlen(*user_input);
		get_next_line(fd, &following_part);
		if (!following_part)
			break ;
		tmp = *user_input;
		*user_input = ft_strjoin(*user_input, following_part);
		free(tmp);
		if (*user_input)
			len = ft_strlen(*user_input);
		update_quotes(following_part, &quotes);
		free(following_part);
	}
	if (errno)
		exit_ms(2, "ms_gnl");
	if (*user_input)
		len = ft_strlen(*user_input);
	if (!conserve_nl && *user_input && (*user_input)[len - 1] == '\n')
		(*user_input)[len - 1] = 0;
}
