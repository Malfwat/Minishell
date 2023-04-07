/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:35:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 02:16:41 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

#include <errno.h>
#include <minishell.h>
#include <parsing_ms.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/wait.h>

#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define RED "\033[38;5;9m"
#define LGREY_BG "\033[48;5;235m"
#define LGREY "\033[38;5;235m"
#define LLGREY "\033[38;5;243m"
#define GREEN "\033[38;5;10m"
#define LGREEN "\033[38;5;76m"
#define DGREEN "\033[38;5;22m"
#define LCYAN "\033[38;5;38m"
#define LBLUE "\033[38;5;110m"
#define ENDC "\033[0m"
#define CORNER_LEFT_D "╰"
#define CORNER_LEFT_U "╭"
#define LINE "─"
#define DOTS_START "░▒▓"
#define DOTS_END "▓▒░"

// prompt_utils_0.c

char	*fetch_current_time(void);
char	*fetch_git_cwd_branch_name(void);

// prompt_utils_1.c

void	ft_memset_uni(char *dest, char *c, int len);
int		fetch_term_width(void);
char	*get_cwd_path_since_home(void);

// build_prompt_0.c

void	build_prompt_user(char **prompt, t_prompt params);
void	build_prompt_start_delim(char **prompt);
void	build_prompt_cwd(char **prompt, t_prompt params);
void	build_prompt_git(char **prompt, t_prompt params);
void	build_prompt_time(char **prompt, t_prompt params);

// build_prompt_1.c

void	build_prompt_exit_status(char **prompt, t_prompt params);
bool	build_prompt_mid_delim(char **prompt, t_prompt params);
void	build_prompt_end_delim(char **prompt);
char	*build_prompt(t_prompt params);
bool	refresh_prompt_param(t_prompt *lst);

#endif /* PROMPT_H */