/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_gnl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 23:16:27 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 23:17:06 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_define.h>

static void	ms_hd_gnl_join_next_line(char **user_input, t_fd fd, int *len)
{
	char	*tmp;
	char	*following_part;

	(*user_input)[*len - 1] = 0;
	*len = ft_strlen(*user_input);
	(*user_input)[*len - 1] = 0;
	write(g_ms_params.stdin_fileno, "> ", 2);
	get_next_line(fd, &following_part);
	if (!following_part)
	{
		*user_input = ft_strjoin(*user_input, "\n");
		write(g_ms_params.stdin_fileno, "\n", 1);
	}
	else
	{
		tmp = *user_input;
		*user_input = ft_strjoin(*user_input, following_part);
		free(tmp);
	}
	if (*user_input)
		*len = ft_strlen(*user_input);
	free(following_part);
}

void	ms_hd_gnl(t_fd fd, char **user_input)
{
	int		len;

	get_next_line(fd, user_input);
	len = 0;
	if (*user_input)
		len = ft_strlen(*user_input);
	while (*user_input && len >= 2 && (*user_input)[len - 1] == '\n' \
		&& (*user_input)[len - 2] == '\\')
		ms_hd_gnl_join_next_line(user_input, fd, &len);
}
