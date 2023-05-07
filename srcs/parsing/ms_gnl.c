/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 14:18:20 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 22:47:59 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_readline.h>
#include <minishell.h>

static bool	join_next_line(char **user_input, t_fd fd, char *quotes)
{
	char	*tmp;
	char	*following_part;

	get_next_line(fd, &following_part);
	if (!following_part)
		return (false);
	tmp = *user_input;
	*user_input = ft_strjoin(*user_input, following_part);
	free(tmp);
	update_quotes(following_part, quotes);
	free(following_part);
	return (true);
}

void	ms_gnl(t_fd fd, char **user_input, bool conserve_nl)
{
	int		len;
	char	quotes;

	get_next_line(fd, user_input);
	len = 0;
	if (*user_input)
		len = ft_strlen(*user_input);
	quotes = 0;
	update_quotes(*user_input, &quotes);
	while (*user_input && quotes)
		if (!join_next_line(user_input, fd, &quotes))
			break ;
	if (errno)
		exit_ms(2, "ms_gnl");
	if (*user_input)
		len = ft_strlen(*user_input);
	if (!conserve_nl && *user_input && (*user_input)[len - 1] == '\n')
		(*user_input)[len - 1] = 0;
}
