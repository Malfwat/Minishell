/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 14:29:49 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <struct_ms.h>	
# include <stdbool.h>
# include <errno.h>




// init_t_block.c

t_block		*new_block(void);
t_block		**last_sub(t_block *head);
t_block		**last_pipe(t_block *head);
t_block		**last_sibling(t_block *head);
void		add_block_back(t_block **head, t_block **(*last)(t_block *));

// built_in/

void		pwd(void);
int			cd(t_env_var	*head, char *str);
void		ms_echo(bool nl, int nb, ...);

// manage_io_params.c

void		ft_add_io(t_block *block, char *io);
t_redirect	*new_redirect(char *arg, int mode);
t_redirect	*last_redirect(t_redirect *head);
void		ft_add_redirect(t_redirect **head, char *arg, int mode);

// manage_cmd_args.c

t_arg		*new_cmd_arg(char *arg);
t_arg		*last_arg(t_arg *head);
void		ft_addargs(t_arg **head, char *arg);
char		**build_argv(char *cmd, t_arg **head);


// free_struct_0.c

void		flood_free(t_block *lst);
void		free_redirect(t_redirect *ptr);
void		free_cmd(t_cmd lst);
void		free_t_args(t_arg *ptr);

// free_struct_1.c
void		free_ms_params(t_minishell ms_params);
void		free_env_lst(t_env_var *envp_lst);

// term_utils
bool		toggle_control_character(int control_character, int mode);
bool		save_terminal_params(t_minishell *ms_params);
bool		restore_terminal_params(struct termios saved_term, t_fd stdin_fileno);
void		exit_ms(t_minishell ms_params, int exitv, char *context);
void		ms_perror(char *program, char *subname, char *error);

// exec 

void		free_children(t_pids **children);
bool		store_pid(pid_t pid, t_pids **nursery);
void		infanticides(t_pids *preys);
int			wait_children(t_pids *children);
int			execute_cmds(t_block *block, t_minishell *ms_params);


#endif /* MINISHELL_H */