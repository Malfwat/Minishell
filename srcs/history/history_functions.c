/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:18:56 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 15:31:19 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <get_next_line.h>
#include <readline/history.h>
#include <fcntl.h>

int	get_my_history(void)
{
	char	*str;
	int		fd;
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

void	my_add_history(char *str, int fd)
{
	if (!*str)
		return ;
	write(fd, str, ft_strlen(str));
	write(fd, "\n", 1);
	add_history(str);
}
