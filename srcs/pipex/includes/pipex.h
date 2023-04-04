/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:12:10 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 16:34:20 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <struct_ppx.h>

# ifndef NO_CMD
#  define NO_CMD "command not found: \0"
# endif /*NO_CMD*/
# ifndef NO_PERM
#  define NO_PERM "permission denied: \0"
# endif /*NO_PERM*/
# ifndef NO_FILE
#  define NO_FILE "no such file or directory: "
# endif /*NO_FILE*/
# ifndef IS_DIR
#  define IS_DIR ": is a directory\n"
# endif /*IS_DIR*/

// free_func.c

void	free_big_tab(char ***tab);
void	free_tab(char **tab);
void	ending(char *str, int **pipefds);
void	error_pipe(t_cmd *cmd, int **pipefds);

// init.c

void	add_slash(char **paths);
char	**get_paths(char **env);
int		get_cmd(int ac, char **av, char **env, t_cmd **cmd);
t_task	init_t_stack(int ac, char **av);

// parsing.c

int		check_cmd(char **path, t_cmd *cmd);
int		check_exe(t_cmd *cmd);

// child_0.c

void	child(int **pipefds, t_task task, int size);
int		before_exec(int *in_out, int i, int *pfds, int size);

// child_1.c

void	print_error_case(t_cmd *cmd);
void	print_error_output(char *str);
void	exec_cmd(t_cmd *cmd, int **pfds, int i, int size);
int		parent_chore(t_task task, int **pfds, int i, int size);

// pipex.c

int		here_doc(char *limiter, t_cmd *cmd);
int		x_pipe(t_task task);

// lst.c

void	f_err_get_cmd(t_cmd *cmd);
int		add_cmd(t_cmd **lst, char **tab);
int		size_cmd(t_cmd *cmd);

// manage_file.c

int		get_infiles(t_task task, int *test2);
int		get_outfiles(t_task task, int *test2);

#endif /*PIPEX_H*/
