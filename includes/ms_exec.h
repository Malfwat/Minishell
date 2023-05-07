/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:20:04 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 22:17:57 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_EXEC_H
# define MS_EXEC_H

# include <libft.h>
# include <minishell.h>
# include <ms_define.h>
# include <ms_struct.h>

// flag_case.c

void		flag_case(char **av);

// children_functions.c

int			wait_children(void);
void		free_children(t_pids **children);
void		infanticides(t_pids *preys);
bool		store_pid(pid_t pid, t_pids **nursery);

// execute_cmd.c

bool		create_pipe(t_block *blck);
void		manage_subshell(t_block *block);
pid_t		create_subshell(t_block *block);
void		execute_t_block_cmd(t_block *block);
void		child_worker(t_block *blck, t_exec_vars exc_vrs);

// execute_utils_0,c

bool		my_dup(t_block *block);
t_exec_vars	init_exec_vars(t_block *block);
t_block		*find_next_executable_block(t_block *block);
void		free_exec_vars(t_exec_vars exec_vars);
bool		create_pipe(t_block *blck);

// execute_utils_1.c

t_block		*find_next_block(t_block *block, bool ignore_sub);
void		join_splitted_arg_test(t_args **args, \
t_env *env, t_s_arg *arg, bool ch);
bool		rebuild_args(t_args **head, t_env *envp);
void		close_sub_fds(t_block *head);
void		free_exec_vars(t_exec_vars exec_vars);

// execute_utils_2.c

void		close_sub_fds(t_block *head);
t_args		*array_to_t_args(char **tab);
bool		rebuild_args(t_args **head, t_env *envp);
void		free_exec_vars(t_exec_vars exec_vars);

// init_exec_vars_io.c

char		**build_path(void);
bool		init_exec_io(t_block *block);
char		**build_argv(t_args **head, char **path, t_env *envp);
char		**build_envp(t_env *envp);
t_exec_vars	init_exec_vars(t_block *block);

void		handle_execve_failure(char *program_name);

// execution.c

void		child_worker(t_block *blck, t_exec_vars exc_vrs);
void		puppet_child(t_block *blck, t_exec_vars exc_vrs);
void		launch_cmd(t_block *block);

// add_color.c

void		check_for_color(t_args **head);
bool		is_colorable(char *str);

int			t_arg_lst_len(t_args *head);
void		set_env_exit_var(int value);

#endif /* EXEC_MS_H */