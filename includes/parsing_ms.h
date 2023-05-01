/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ms.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:10:20 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/01 08:49:44 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_MS_H
# define PARSING_MS_H
# include <stdlib.h>
# include <sys/types.h>
# include <struct_ms.h>
# include <errno.h>
# include <stdbool.h>

// parenthesis.c

int		find_closing_parenthesis(char *str);
bool	check_parenthesis_param(char *str, int *i, char **new_line, int *type);

// parsing.c

void	*get_next_param(char *str, int *i, int *type);
bool	is_delimiter(char *str, int *storage);
bool	is_valid_param(void *param, int type, t_block *block);
bool	parse_cmds(t_block **curr_block, char *cmd_line);

// check_cmd.c

bool	get_cmd_path(char **path, char **cmd, char **dest);

// utils.c 

int		pass_ws_and_delim(char *str, int type);
int		count_chars_inside_quotes(char **str, char c);
int		copy_chars_inside_quotes(char *src, char c, char **dest);
int		count_param_length(char *str, char *charset, int *size);
int		pass_whitespaces(char *str);

// word.c

bool	check_word_param(char *str, int *i, int *type, t_s_arg **arg);

// input_output.c

int		ft_substr_io_param(char *src, char *dest);
bool	check_io_param(char *str, int *i, int *type, t_s_arg **arg);

// io_functions.c

// int		heredoc(char *limiter);
int		input_manager(t_redirect *ptr, t_fd *fd, t_block *block, \
t_env *envp);
int		output_manager(t_redirect *ptr, t_fd *fd, t_env *envp);

bool	is_delim(char *str);

bool	check_and_store_delimiter(char *str, int *storage);
void	syntax_error(int err, void *comment, int type, char *cmd_line);
void	print_syntax_error(int type, char *str);
void	print_heredoc_syntax_error(char *heredoc_delimiter);
bool	parse_user_input(char *user_input);

#endif /* PARSING_MS_H */