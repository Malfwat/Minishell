/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 16:06:51 by amouflet          #+#    #+#             */
/*   Updated: 2023/04/07 21:42:04 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	here_doc(char *limiter, t_cmd *cmd)
{
	int		test[2];
	char	*str;
	int		size;
	char	*tmp;

	if (pipe(test) == -1)
		return (f_err_get_cmd(cmd->begin), -1);
	str = get_next_line(0);
	size = ft_strlen(str);
	tmp = ft_strjoin(limiter, "\n");
	while (ft_strncmp(str, tmp, size))
	{
		write(test[1], str, size);
		free(str);
		str = get_next_line(0);
		size = ft_strlen(str);
	}
	free(str);
	free(tmp);
	close(test[1]);
	return (test[0]);
}

int	x_pipe(t_task task)
{
	int		*pipefds[2];
	int		size;
	int		tmp[2];
	int		tmp2[2];

	size = size_cmd(task.cmd) - 1;
	if (size == -1)
		return (1);
	if (pipe(tmp))
		return (1);
	pipefds[0] = tmp;
	pipefds[1] = tmp2;
	child((int **)pipefds, task, size);
	return (0);
}
