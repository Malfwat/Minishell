/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 13:35:07 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/09 23:38:39 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ms_struct.h>
#include <ms_define.h>
#include <minishell.h>
#include <readline/readline.h>
#include <ms_readline.h>
#include <ms_signal.h>
#include <ms_prompt.h>

void	rdl_backslash(char **last_read, char *quote)
{
	char	*tmp;
	int		len;
	char	*new_read;

	if (!*last_read || !**last_read || *quote)
		return ;
	len = ft_strlen(*last_read);
	if (len >= 1 && (*last_read)[len - 1] != '\\')
		return ;
	tmp = *last_read;
	len = len - 1;
	new_read = readline("> ");
	if (!new_read)
		return ;
	(*last_read)[ft_strlen(*last_read) - 1] = 0;
	*last_read = ft_strjoin(*last_read, new_read);
	free(new_read);
	free(tmp);
	update_quotes(&(*last_read)[len], quote);
}

void	error_ms_readline(const char quote)
{
	ms_perror("minishell", "unexpected EOF while looking for matching", \
	(char []){quote, 0});
	ms_perror("minishell", "syntax error", "unexpected end of file");
	my_close(g_ms_params.readline_pipe[1], -2);
	errno = 0;
	exit_ms(2, "ms_readline");
}

void	rdl_write_in_pipe(char	*str, char quote)
{
	bool	erase_slash;

	erase_slash = 0;
	if (str)
	{
		if ((ft_strlen(str) > 1 && str[ft_strlen(str) - 1] == '\\') && !quote)
			erase_slash = 1;
		write(g_ms_params.readline_pipe[1], str, ft_strlen(str) - erase_slash);
		if (quote)
			write(g_ms_params.readline_pipe[1], "\n", 1);
	}
	else if (!str && !quote)
	{
		write(g_ms_params.stdin_fileno, "exit\n", 5);
		my_close(g_ms_params.readline_pipe[1], -2);
		return (errno = 0, exit_ms(1, "ms_readline"));
	}
}

void	readline_child(void)
{
	char				*tmp;
	char				quotes;

	signal(SIGINT, handler_readline);
	my_close(g_ms_params.readline_pipe[0], -2);
	if (!refresh_prompt_param(&g_ms_params.prompt_params, \
		g_ms_params.last_exit_code))
	{
		my_close(g_ms_params.readline_pipe[1], -2);
		exit_ms(0, "prompt1");
	}
	ensure_prompt_position();
	g_ms_params.ms_prompt = build_prompt(&g_ms_params.prompt_params, P_HEADER);
	free_prompt_params(&g_ms_params.prompt_params);
	quotes = 0;
	tmp = readline(g_ms_params.ms_prompt);
	if (errno == EINTR)
		errno = 0;
	quotes = check_for_quotes(tmp, quotes);
	free(g_ms_params.ms_prompt);
	g_ms_params.ms_prompt = NULL;
	ms_readline(tmp, quotes);
	my_close(g_ms_params.readline_pipe[1], -2);
	exit_ms(0, "fork_readline");
}

void	ms_readline(char *tmp, char quote)
{
	rdl_backslash(&tmp, &quote);
	rdl_write_in_pipe(tmp, quote);
	if (!quote && (!*tmp || tmp[ft_strlen(tmp) - 1] != '\\'))
		return (free(tmp));
	free(tmp);
	tmp = NULL;
	if (quote == '\'')
		tmp = readline("quote> ");
	else if (quote == '"')
		tmp = readline("dquote> ");
	else
		tmp = readline("> ");
	if (errno == EINTR)
		errno = 0;
	if (!tmp)
		error_ms_readline(quote);
	update_quotes(tmp, &quote);
	return (ms_readline(tmp, quote));
}
