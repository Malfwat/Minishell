/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:18:56 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 16:52:07 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <get_next_line.h>
#include <readline/history.h>
#include <fcntl.h>
#include <ms_define.h>

int	get_my_history(void)
{
	char	*str;
	t_fd	fd;
	char	*tmp;

	tmp = getenv("HOME");
	if (!tmp)
		return (-1);
	tmp = ft_strjoin(tmp, "/.my_history");
	fd = open(tmp, O_RDWR | O_APPEND | O_CREAT, 0644);
	free(tmp);
	if (fd == -1)
		return (-1);
	get_next_line(fd, &str);
	while (str)
	{
		if (str[ft_strlen(str) - 1] == '\n')
			str[ft_strlen(str) - 1] = 0;
		add_history(str);
		free(str);
		get_next_line(fd, &str);
	}
	return (fd);
}

void	ms_add_history(char *str, t_fd fd)
{
	if (!*str)
		return ;
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	add_history(str);
}
