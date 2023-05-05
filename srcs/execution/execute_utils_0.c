/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_0.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 04:52:13 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:18:03 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_exec.h>

bool	my_dup(t_block *block)
{
	if (block->io_tab[0] > 2)
	{
		if (dup2(block->io_tab[0], 0) == -1 || close(block->io_tab[0]) == -1)
			return (false);
	}
	if (block->io_tab[1] > 2)
	{
		if (dup2(block->io_tab[1], 1) == -1 || close(block->io_tab[1]) == -1)
			return (false);
	}
	if (block->pipe_next)
		my_close(block->pipe_next->io_tab[0], -2);
	return (true);
}

t_block	*find_next_executable_block(t_block *block)
{
	int		exit_value;

	exit_value = extract_exit_code(block->cmd.exit_value);
	while (block)
	{
		if (block->operator == AND_OPERATOR && exit_value == SUCCESS)
			return (find_next_block(block, true));
		else if (block->operator == OR_OPERATOR && exit_value > 0)
			return (find_next_block(block, true));
		else if (block->operator == PIPE_OPERATOR)
			return (find_next_block(block, true));
		else if (block->operator == SEMI_COLON)
			return (find_next_block(block, true));
		else
			block = find_next_block(block, true);
	}
	return (NULL);
}

bool	create_pipe(t_block *blck)
{
	t_block	*nxt_blck;
	int		tube[2];

	nxt_blck = find_next_block(blck, true);
	if (pipe(tube) == -1)
		return (false);
	if (nxt_blck->io_tab[0] == INIT_FD_VALUE || nxt_blck->io_is_overwritable[0])
	{
		if (nxt_blck->io_tab[0] > 2)
			my_close(nxt_blck->io_tab[0], -2);
		nxt_blck->io_tab[0] = tube[0];
		nxt_blck->io_is_overwritable[0] = true;
	}
	else
		my_close(tube[0], -2);
	if (blck->io_tab[1] == INIT_FD_VALUE || !blck->io_is_overwritable[1])
	{
		if (blck->io_tab[1] > 2)
			my_close(blck->io_tab[1], -2);
		blck->io_tab[1] = tube[1];
		blck->io_is_overwritable[1] = true;
	}
	else
		my_close(tube[1], -2);
	return (true);
}
