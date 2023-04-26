/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ms.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:20:04 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/26 14:46:53 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_MS_H
# define EXEC_MS_H

# include <libft.h>
# include <minishell.h>
# include <ms_define.h>
# include <struct_ms.h>

// children_functions.c

int			wait_children(t_minishell *ms_params);
void		free_children(t_pids **children);
void		infanticides(t_pids *preys);
bool		store_pid(pid_t pid, t_pids **nursery);

// execute_cmd.c

bool		create_pipe(t_block *blck);
void		manage_subshell(t_block *block, t_minishell *ms_params);
pid_t		create_subshell(t_block *block, t_minishell *ms_prm);
void		execute_t_block_cmd(t_block *block, t_minishell *ms_params);
void		child_worker(t_block *blck, t_minishell *ms_params, \
t_exec_vars exc_vrs);

// execute_utils_0,c

bool		my_dup(t_block *block);
t_exec_vars	init_exec_vars(t_minishell ms_params, t_block *block);
t_block		*find_next_executable_block(t_block *block);
bool		create_pipe(t_block *blck);

// execute_utils_1.c

t_block		*find_next_block(t_block *block, bool ignore_sub);
bool		rebuild_args(t_args **head, t_env_var *envp);
void		close_sub_fds(t_block *head);
void		free_exec_vars(t_exec_vars exec_vars);

// init_exec_vars_io.c

char		**build_path(t_minishell ms_params);
bool		init_exec_io(t_block *block, t_minishell *ms_params);
char		**build_argv(t_args **head, char **path, t_env_var *envp);
char		**build_envp(t_env_var	*envp);
t_exec_vars	init_exec_vars(t_minishell ms_params, t_block *block);

void		handle_execve_failure(t_minishell ms_params, char *program_name);

#endif /* EXEC_MS_H */