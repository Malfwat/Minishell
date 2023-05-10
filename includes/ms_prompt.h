/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:35:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:05:09 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PROMPT_H
# define MS_PROMPT_H

# include <errno.h>
# include <minishell.h>
# include <ms_parsing.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/wait.h>

// prompt_utils_0.c

char			*fetch_current_time(void);
char			*fetch_git_cwd_branch_name(void);

// prompt_utils_1.c

void			ft_memset_uni(char *dest, char *c, int len);
int				fetch_term_width(void);
char			*get_cwd_path_since_home(void);

// prompt_utils_2.c

void			ls_free_pargs(t_prompt_blocks *pargs);
char			*strjoin_pargs(t_prompt_blocks *pargs);
void			get_pargs_len(char c, int *total, int *i, \
bool *control_sequence);
void			ls_edit_p_args_if(t_prompt_blocks *parg, int type, \
char *str, int dlen);
int				pargs_len(t_prompt_blocks *pargs);

// prompt_utils_3.c

void			build_upper_prompt(t_prompt *params, t_prompt_blocks **pargs);
int				current_mid_delim_len(t_prompt_blocks *pargs);
void			check_prompt_width(t_prompt_blocks *pargs, t_prompt *params);
bool			refresh_prompt_param(t_prompt *lst, int last_exit_code);
int				extract_exit_code(int status);

// build_prompt_0.c

void			build_prompt_user(t_prompt_blocks **pargs, t_prompt *params);
void			build_prompt_start_delim(t_prompt_blocks **pargs);
void			build_prompt_cwd(t_prompt_blocks **pargs, t_prompt *params);
void			build_prompt_git(t_prompt_blocks **pargs, t_prompt *params);
void			build_prompt_time(t_prompt_blocks **pargs, t_prompt *params);

// build_prompt_1.c

void			build_prompt_exit_status(t_prompt_blocks **pargs, \
t_prompt *params);
char			*build_mid_delim(int len);
void			build_prompt_mid_delim(t_prompt_blocks **pargs, int len);
void			build_prompt_end_delim(t_prompt_blocks **pargs);
char			*build_prompt(t_prompt *params, bool side);
bool			refresh_prompt_param(t_prompt *lst, int last_exit_code);

void			ls_p_args_addback(t_prompt_blocks **head, t_prompt_blocks *new);
t_prompt_blocks	*ls_new_p_args(char type, char *str, int delim_len);

#endif /* PROMPT_H */