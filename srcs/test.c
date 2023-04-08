/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/07 22:56:45 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <prompt.h>

#include <signal_ms.h>
#include <parsing_ms.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <minishell.h>
#include <history.h>

int	main(int ac, char **av)
{
	char		*res;
	int			type;
	t_block		*head;
	// t_prompt	prompt_params;

	// set_sig_handler();
	// if (!refresh_prompt_param(&prompt_params))
		// return (1);
	res = NULL;
	type = -1;
	(void)ac;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	// int fd = get_my_history();
	// if (fd == -1)
		// return (2);
	// readline(build_prompt(prompt_params));
	// my_add_history(rl_line_buffer, fd);
	char *str =  ft_strdup(av[1]);
	head = new_block();

	if (parse_cmds(&head, str))
		execute_cmds(head, );
	if (errno)
		return (perror("minishell"), flood_free(head), 0);
	flood_free(head);
	// free(rl_prompt);
	// close(fd);
	printf("[%d] res: '%s'\n", type, res);
}


int	here_doc(char *limiter, t_cmd *cmd)
{
	int		test[2];
	char	*str;
	int		size;
	char	*tmp;

	if (pipe(test) == -1)
		return (f_err_get_cmd(cmd->begin), -1);
	str = get_next_line(0);
	size = ft_strlen(str);
	tmp = ft_strjoin(limiter, "\n");
	while (ft_strncmp(str, tmp, size))
	{
		write(test[1], str, size);
		free(str);
		str = get_next_line(0);
		size = ft_strlen(str);
	}
	free(str);
	free(tmp);
	close(test[1]);
	return (test[0]);
}

void	input_manager(t_redirect *ptr, int *fd)
{
	if (ptr->file_name)
		ptr->fd = open(ptr->file_name, O_RDONLY);
	if (ptr->fd != -1)
		*fd = ptr->fd;
	ptr->errno_value = errno;
}

void	output_manager(t_redirect *ptr, int *fd)
{
	if (ptr->append)
		ptr->fd = open(ptr->file_name, O_WRONLY | O_CREAT | O_APPEND, 00644);
	else
		ptr->fd = open(ptr->file_name, O_WRONLY | O_TRUNC | O_CREAT, 00644);
	if (ptr->fd != -1)
		*fd = ptr->fd;
	ptr->errno_value = errno;
}

bool	heredoc_manager(t_redirect *ptr)
{
	while (ptr)
	{
		ptr->fd = heredoc(ptr->heredoc);
		if (!ptr->fd == -1)
			return (false);
		ptr = ptr->next;
	}
	return (true);
}

int	*io_manager(t_block *block)
{
	t_redirect	*tmp;

	if (!heredoc_manager(block->heredoc))
		return (NULL);
	tmp = block->io_redirect;
	while (tmp && !errno)
	{
		if (tmp->mode == INPUT_MODE)
			input_manager(tmp, &block->io_tab[0]);
		else if (tmp->mode == OUTPUT_MODE)
			ouput_manager(tmp, &block->io_tab[1]);
		tmp = tmp->next;
	}
	if (errno)
		return (NULL);
}

void	execute_t_block_cmd(t_block *block, int *status, t_env_var *envp_lst)
{
	char **argv;
	char **envp;
	
	argv = build_argv(block->cmd.name, block->cmd.args);
	envp = build_envp(envp_lst);
	if (!argv || !envp)
		return (free(argv), free(envp), perror("minishell"));
	block->cmd.pid = fork();
	if (!block->cmd.pid)
	{
		if (execve(argv[0], argv, envp) == -1)
			return (free(envp), free(argv), exit_minishell());
	}
	free(envp);
	free(argv);
}

int	execute_cmds(int *status, t_block *block, int *pipe, t_minishell ms_params)
{
	if (block->do_not_execute)
		return (ft_putstr_fd(block->do_not_execute, 2), false);
	else if (block->subshell_command)
		execute_cmds(status, block, pipe, ms_params);
	else
	{
		while (block->pipe_next)
		{
			execute_t_block_cmd(block, &status, ms_params.envp);
			// pipex
		}
		if (block->operator == AND_OPERATOR && *status != 0)
			return (0);
		else
		{
			
		}
	}
}