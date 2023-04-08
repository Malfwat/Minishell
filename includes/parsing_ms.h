/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ms.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:10:20 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/08 08:06:52 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# define SINGLE_DELIM " \t><|;"
# include <stdlib.h>
# include <sys/types.h>
# include <struct_ms.h>
# include <errno.h>
# include <stdbool.h>
# define INPUT_MODE 0
# define OUTPUT_MODE 1
# define HEREDOC 0
# define FILE_INPUT 1

// parenthesis.c

int		find_closing_parenthesis(char *str);
bool	check_parenthesis_param(char *str, int *i, char **new_line, int *type);

// parsing.c

char	*get_next_param(char *str, int *i, int *type);
bool	is_delimiter(char *str, int *storage);
bool	is_valid_param(char *param, int type, t_block *block, char **path);
bool	parse_cmds(t_block **curr_block, char *cmd_line, char **path);

// check_cmd.c

char	*check_cmd(char **path, char *cmd);


// utils.c 

int		pass_ws_and_delim(char *str, int type);
int		count_chars_inside_quotes(char **str, char c);
int		copy_chars_inside_quotes(char *src, char c, char **dest);
int		count_param_length(char *str, char *charset, int *size);
int		pass_whitespaces(char *str);

// word.c

bool	check_word_param(char *str, int *i, char **new_line, int *type);
int		ft_substr_word_param(char *src, char *dest);

// input_output.c

int		ft_substr_io_param(char *src, char *dest);
bool	check_io_param(char *str, int *i, char **new_line, int *type);

// io_functions.c

int		heredoc(char *limiter);
bool	heredoc_manager(t_redirect *ptr);
void	input_manager(t_redirect *ptr, int *fd);
bool	io_manager(t_block *block);
void	output_manager(t_redirect *ptr, int *fd);

#endif /* PARSING_H */