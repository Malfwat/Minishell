/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:12:41 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <struct.h>

enum e_operator
{
	AND_OPERATOR,
	OR_OPERATOR,
	PIPE_OPERATOR,
	SEMI_COLON
};

// init_t_block.c

t_block	*new_block(char *line);
t_block	*last_sub(t_block *head);
t_block	*last_pipe(t_block *head);
t_block	*last_sibling(t_block *head);
int		add_block_back(t_block **head, char *line, t_block *(*last)(t_block *));


#endif /* MINISHELL_H */