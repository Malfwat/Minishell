/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 23:09:42 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 16:33:28 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	error_pipe(t_cmd *cmd, int **pipefds)
{
	int	i;
	int	j;

	i = 0;
	f_err_get_cmd(cmd->begin);
	while (i < 2)
	{
		j = 0;
		while (j < 2)
		{
			if (pipefds[i][j])
				close(pipefds[i][j]);
			j++;
		}
		i++;
	}
	ft_putstr_fd("pipe failed\n", 2);
	exit(2);
}

void	ending(char *str, int **pipefds)
{
	if (pipefds[0][1] == -1)
	{
		if (access(str, F_OK))
		{
			ft_putstr_fd(NO_FILE, 2);
			ft_putendl_fd(str, 2);
		}
		else if (access(str, R_OK))
		{
			ft_putstr_fd(NO_PERM, 2);
			ft_putendl_fd(str, 2);
		}
		else
			print_error_output(str);
	}
	else
		close(pipefds[0][1]);
	if (pipefds[0][0] != 0)
		close(pipefds[0][0]);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_big_tab(char ***tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		free_tab(tab[i]);
		i++;
	}
	if (tab)
		free(tab);
}
