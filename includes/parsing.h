/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:10:20 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:20:44 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# define SINGLE_DELIM " \t><|;"
# include <stdlib.h>
# include <stdbool.h>

// parenthesis.c

int		find_closing_parenthesis(char *str);
bool	check_parenthesis_param(char *str, int *i, char **new_line);


// parsing.c

int	count_chars_inside_quotes(char **str, char c);
int	copy_chars_inside_quotes(char *src, char c, char **dest);
int	count_param_length(char *str, char *charset, int *size);
char	*get_next_param(char *str, int *i);

// word.c

bool	check_word_param(char *str, int *i, char **new_line);
int	ft_substr_word_param(char *src, char *dest);

// input_output.c

int	ft_substr_io_param(char *src, char *dest);
bool	check_io_param(char *str, int *i, char **new_line);

#endif /* PARSING_H */