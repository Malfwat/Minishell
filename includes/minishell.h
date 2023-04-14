/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/14 20:28:53 by hateisse         ###   ########.fr       */
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

void		ft_add_io(t_block *block, t_split_arg *io);
t_redirect	*new_redirect(t_split_arg *arg, int mode);
t_redirect	*last_redirect(t_redirect *head);
void		ft_add_redirect(t_redirect **head, t_split_arg *arg, int mode);
bool		hd_manager(t_block *block);

// manage_cmd_args.c

t_args		*new_cmd_arg(char *arg);
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
bool		restore_terminal_params(struct termios saved_term, t_fd stdin_fileno);
void		exit_ms(t_minishell ms_params, int exitv, char *context);
void		ms_perror(char *program, char *subname, char *error);

// exec 

void		free_children(t_pids **children);
bool		store_pid(pid_t pid, t_pids **nursery);
void		infanticides(t_pids *preys);
int			wait_children(t_pids *children);
int			execute_cmds(t_block *block, t_minishell *ms_params);


char		*join_splitted_arg(t_split_arg *arg, t_env_var *envp, bool interpret);



#endif /* MINISHELL_H */