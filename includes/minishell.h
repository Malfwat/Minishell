/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 17:39:10 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <struct_ms.h>	
# include <stdbool.h>
# include <ms_define.h>
# include <errno.h>

// init_t_block.c

t_block		*new_block(void);
t_block		**last_sub(t_block *head);
t_block		**last_pipe(t_block *head);
t_block		**last_sibling(t_block *head);
void		add_block_back(t_block **head, t_block **(*last)(t_block *));

// built_in/

void		pwd(t_fd fd);
void		cd(t_minishell *ms_params, t_env_var *head, char **tab);
void		ms_echo(char **tab, t_fd fd);

// manage_io_params.c

void		ft_add_io(t_block *block, t_split_arg *io);
t_redirect	*new_redirect(t_split_arg *arg, int mode);
t_redirect	*last_redirect(t_redirect *head);
void		ft_add_redirect(t_redirect **head, t_split_arg *arg, int mode);
int			hd_manager(t_block *block);

// manage_cmd_args.c

t_args		*new_cmd_arg(t_split_arg *arg);
t_args		*last_args(t_args *head);
void		ft_ls_t_args_addback(t_args **head, t_split_arg *arg);
char		**build_argv(t_args **head);

// free_struct_0.c

void		flood_free(t_block *lst);
void		free_redirect(t_redirect *ptr);
void		free_cmd(t_cmd lst);
void		free_t_args(t_args *ptr);

// free_struct_1.c

void		free_ms_params(t_minishell ms_params);
void		free_env_lst(t_env_var *envp_lst);
void		free_prompt_params(t_prompt *lst);
void		free_t_split_arg(t_split_arg **arg);

// term_utils

bool		toggle_control_character(int control_character, int mode);
bool		save_terminal_params(t_minishell *ms_params);
bool		restore_terminal_params(struct termios saved_term, \
t_fd stdin_fileno);
void		exit_ms(t_minishell ms_params, int exitv, char *context);
void		ms_perror(char *program, char *subname, char *error);

// exec 

void		free_children(t_pids **children);
bool		store_pid(pid_t pid, t_pids **nursery);
void		infanticides(t_pids *preys);
int			wait_children(t_minishell *ms_params);
int			execute_commands(t_block *block, t_minishell *ms_params);

int			extract_exit_code(int status);

char		*join_splitted_arg(t_split_arg *arg, t_env_var *envp, \
bool interpret);

// meta_char.c

char		*interpret_dollars(t_split_arg *arg, t_env_var *envp);
void		update_t_args(t_args **args);

void		handler_func(int num);

//   init_t_args.c

void		ft_ls_t_args_addback(t_args **head, t_split_arg *arg);
t_args		*last_args(t_args *head);
void		ft_addarg_front(t_args **head, t_split_arg *arg);
void		insert_t_args(t_args **head, t_args *current, t_args *new_lst);
t_args		*new_cmd_arg(t_split_arg *arg);

// init_t_split_args.c

void		free_t_split_arg(t_split_arg **arg);
char		*join_splitted_arg(t_split_arg *arg, t_env_var *envp, \
bool interpret);
void		ls_split_args_addback(t_split_arg **head, t_split_arg *new);
t_split_arg	*ls_split_args_new(char *data, char scope);

// utils_1.c

char		is_quote(char c);
bool		is_delim(char *str);
int			slice_next_part(char *src, t_split_arg **last_args, char quotes);
void		free_next_param(void **ptr, int type);
bool		is_parenthesis_empty(char *str);

// io_functions.c

int			input_manager(t_redirect *ptr, t_fd *fd, t_block *block, \
t_env_var *envp);
int			output_manager(t_redirect *ptr, t_fd *fd, t_env_var *envp);
int			hd_manager(t_block *block);
int			heredoc(char *limiter);

// wildcard

bool		manage_wildcard(t_args **head, char *str);
void		split_path_pattern(char *str, char **path, char **pattern);
t_args		*wildcard(char *dir, char *pattern);
bool		compare_wildcard(char *pattern, char *str);

// init_shell.c

void		ensure_prompt_position(void);
void		init_prompt(t_minishell *ms_params, char **user_input);
bool		init_minishell(t_minishell *ms_params, char **envp);
int			get_cursor_position(void);


bool	is_builtin(char *str);
void	exec_builtin(t_block *block, t_minishell *ms_params, t_exec_vars vars);

#endif /* MINISHELL_H */