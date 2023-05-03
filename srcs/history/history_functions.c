/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:18:56 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/01 08:34:30 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <get_next_line.h>
#include <readline/history.h>
#include <fcntl.h>
#include <minishell.h>
#include <struct_ms.h>
#include <ms_define.h>

int	fill_history(int fd)
{
	char	*str;

	get_next_line(fd, &str);
	while (str)
	{
		if (str[ft_strlen(str) - 1] == '\n')
			str[ft_strlen(str) - 1] = 0;
		add_history(str);
		free(g_ms_params.prev_line);
		g_ms_params.prev_line = ft_strdup(str);
		if (!g_ms_params.prev_line)
			return (free(str), my_close(fd, -2), -1);
		free(str);
		get_next_line(fd, &str);
	}
	return (fd);
}

int	get_my_history(void)
{
	t_fd	fd;
	char	*tmp;

	tmp = getenv("HOME");
	if (!tmp)
		return (-1);
	tmp = ft_strjoin(tmp, "/.my_history");
	if (!tmp)
		return (-1);
	fd = open(tmp, O_RDWR | O_APPEND | O_CREAT, 0644);
	free(tmp);
	if (fd == -1)
		return (-1);
	return (fill_history(fd));
}

void	ms_add_history(char *str)
{
	static char	*prev_line;
	int			i;
	t_fd		fd;

	fd = g_ms_params.history_fd;
	if (!*str || (g_ms_params.prev_line \
		&& !ft_strcmp(g_ms_params.prev_line, str)))
		return ;
	i = 0;
	while (str[i] && ft_strchr(" \t", str[i]))
		i++;
	if (!str[i])
		return ;
	prev_line = ft_strdup(str);
	if (!prev_line)
		exit_ms(1, "history");
	free(g_ms_params.prev_line);
	g_ms_params.prev_line = prev_line;
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	add_history(str);
}
