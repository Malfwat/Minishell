/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/05 22:58:40 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <struct_ms.h>	
# include <stdbool.h>
# include <errno.h>

enum e_operator
{
	AND_OPERATOR = 1,
	OR_OPERATOR,
	PIPE_OPERATOR,
	SEMI_COLON
};

enum e_type
{
	INPUT_OUTPUT,
	PARENTHESIS,
	CMD_ARG
};

enum e_errors
{
	CMD_SYNTAX_ERR
};

// init_t_block.c

t_block	*new_block(void);
t_block	*last_sub(t_block *head);
t_block	*last_pipe(t_block *head);
t_block	*last_sibling(t_block *head);
void	add_block_back(t_block **head, t_block *(*last)(t_block *));

// built_in/

void	pwd(void);
int		cd(t_env_var	*head, char *str);
void	echo(bool nl, int nb, ...);

// struct_utils

void	ft_add_io(t_block *block, char *io);
void	ft_addargs(t_arg **head, char *arg);
char	**t_arg_to_array(t_arg *head);


#endif /* MINISHELL_H */