/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:35:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 17:08:18 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROMPT_H
# define PROMPT_H

# include <errno.h>
# include <minishell.h>
# include <parsing_ms.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/wait.h>




// prompt_utils_0.c

char	*fetch_current_time(void);
char	*fetch_git_cwd_branch_name(void);

// prompt_utils_1.c

void	ft_memset_uni(char *dest, char *c, int len);
int		fetch_term_width(void);
char	*get_cwd_path_since_home(void);

// build_prompt_0.c

void	build_prompt_user(t_prompt_blocks **pargs, t_prompt *params);
void	build_prompt_start_delim(t_prompt_blocks **pargs);
void	build_prompt_cwd(t_prompt_blocks **pargs, t_prompt *params);
void	build_prompt_git(t_prompt_blocks **pargs, t_prompt *params);
void	build_prompt_time(t_prompt_blocks **pargs, t_prompt *params);

// build_prompt_1.c

void	build_prompt_exit_status(t_prompt_blocks **pargs, t_prompt *params);
void	build_prompt_mid_delim(t_prompt_blocks **pargs, int len);
void	build_prompt_end_delim(t_prompt_blocks **pargs);
char	*build_prompt(t_prompt *params);
bool	refresh_prompt_param(t_prompt *lst, int last_exit_code);

void			ls_p_args_addback(t_prompt_blocks **head, t_prompt_blocks *new);
t_prompt_blocks	*ls_new_p_args(char type, char *str, int delim_len);

#endif /* PROMPT_H */