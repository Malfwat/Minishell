/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/06 22:34:47 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <libft.h>
#include <minishell.h>
#include <parsing_ms.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/wait.h>
#define BOLD "\033[1m"
#define RED "\033[38;5;9m"
#define LPURPLE "\033[38;5;189m"
#define LGREY_BG "\033[48;5;235m"
#define LGREY "\033[38;5;235m"
#define LLGREY "\033[38;5;243m"
#define GREEN "\033[38;5;10m"
#define LGREEN "\033[38;5;76m"
#define DGREEN "\033[38;5;22m"
#define LCYAN "\033[38;5;38m"
#define LBLUE "\033[38;5;110m"
#define ENDC "\033[0m"
#define FOLDER_ICON "🗀   🖿     🗁"
#define ARROW ""
#define CORNER_LEFT_D "╰"
#define CORNER_LEFT_U "╭"
#define LINE "─"
#define DOTS_START "░▒▓"
#define DOTS_END "▓▒░"
#define ITALIC "\033[3m"

void	build_prompt_exit_status(char **prompt, t_prompt params)
{
	char	*tmp;
	char	*ascii_status;

	ascii_status = ft_itoa(params.last_exit_code);
	if (!ascii_status)
		return ;
	tmp = *prompt;
	if (params.last_exit_code == 0)
		*prompt = ft_strsjoin(7, tmp, LGREY_BG, GREEN, "✔ ", LLGREY, "", ENDC);
	else
		*prompt = ft_strsjoin(8, tmp, RED, LGREY_BG, ascii_status, " ✘ ",
				LLGREY, "", ENDC);
	free(ascii_status);
	free(tmp);
}

char	*fetch_current_time(void)
{
	int		status;
	int		tube[2];
	int		pid;
	char	*res;

	pipe(tube);
	pid = fork();
	if (pid == -1)
		return (NULL);
	else if (pid == 0)
	{
		if (close(STDERR_FILENO) || close(tube[0]) == -1)
			perror("minishell");
		else if (dup2(tube[1], STDOUT_FILENO) == -1)
			perror("minishell");
		else if (execve("/usr/bin/date", (char *[]){"/usr/bin/date", "+%T", NULL}, NULL) == -1)
			perror("minishell");
	}
	else
		waitpid(pid, &status, 0);
	close(tube[1]);
	get_next_line(tube[0], &res);
	close(tube[0]);
	if (*res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}

void	build_prompt_time(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, LGREY_BG, " ", LBLUE, params.time, LLGREY, ENDC);
	free(tmp);
	free(params.time);
}

char	*fetch_git_cwd_branch_name(void)
{
	int		status;
	int		tube[2];
	int		pid;
	char	*res;

	pipe(tube);
	pid = fork();
	if (pid == -1)
		return (NULL);
	else if (pid == 0)
	{
		if (close(STDERR_FILENO) || close(tube[0]) == -1)
			perror("minishell");
		else if (dup2(tube[1], STDOUT_FILENO) == -1)
			perror("minishell");
		else if (execve("/usr/bin/git", (char *[]){"/usr/bin/git", "branch", "--show-current", NULL}, NULL) == -1)
			perror("minishell");
	}
	else
		waitpid(pid, &status, 0);
	close(tube[1]);
	get_next_line(tube[0], &res);
	close(tube[0]);
	if (res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}

void	build_prompt_git(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(11, tmp, LGREY_BG, LLGREY,
			"  ", ENDC, LGREY_BG, "\u2387  ", LGREEN, params.git_branch_name, LLGREY, ENDC);
	free(tmp);
	free(params.git_branch_name);
}

void	build_prompt_cwd(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, BOLD, LGREY_BG, " ", LCYAN, params.cwd, ENDC);
	free(tmp);
	free(params.cwd);
}

void	build_prompt_start_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(6, tmp, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	free(tmp);
}

void	ft_memset_uni(char *dest, char *c, int len)
{
	while (len--)
	{
		*(dest++) = c[0];
		*(dest++) = c[1];
		*(dest++) = c[2];
	}
}

bool	build_prompt_mid_delim(char **prompt, t_prompt params)
{
	char	*tmp;
	char	*delim;
	int		len;

	len = params.term_width - params.width_without_mid_delim - 37;
	delim = NULL;
	if (len > 0)
	{
		delim = ft_calloc(len + 1, sizeof(char) * 3);
		if (!delim)
			return (false);
		ft_memset_uni(delim, "─", len);
	}
	tmp = *prompt;
	*prompt = ft_strsjoin(8, tmp, LGREY, "▓▒░", LLGREY, delim,
			LGREY, "░▒▓", ENDC);
	free(tmp);
	free(delim);
	return (true);
}

void	build_prompt_user(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(8, tmp, ITALIC, LGREY, " ", params.session_user, DGREEN, " $ ", ENDC);
	free(tmp);
}

void	build_prompt_end_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(9, tmp, ENDC, LGREY, "▓▒░", ENDC, LLGREY, "\n╰─",
			BOLD, ENDC);
	free(tmp);
}

char	*build_prompt(t_prompt params)
{
	char	*prompt;

	errno = 0;
	prompt = ft_calloc(1, 1);
	if (prompt)
	{
		build_prompt_start_delim(&prompt);
		build_prompt_cwd(&prompt, params);
		build_prompt_git(&prompt, params);
		build_prompt_mid_delim(&prompt, params);
		build_prompt_exit_status(&prompt, params);
		build_prompt_time(&prompt, params);
		build_prompt_end_delim(&prompt);
		build_prompt_user(&prompt, params);
	}
	if (errno)
		return (free(prompt), NULL);
	return (prompt);
}

char	*get_cwd_path_since_home(void)
{
	char	*home_path;
	char	*cwd;
	char	*home_in_cwd;
	char	*result;

	result = NULL;
	home_path = getenv("HOME");
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	else if (home_path && *home_path)
	{
		home_in_cwd = ft_strnstr(cwd, home_path, ft_strlen(home_path));
		if (home_in_cwd)
		{
			result = ft_strjoin("~", cwd + ft_strlen(home_path));
			return (free(cwd), result);
		}
	}
	return (cwd);
}

int	fetch_term_width(void)
{
	struct winsize	w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		return (-1);
	return (w.ws_col);
}

bool	refresh_prompt_param(t_prompt *lst)
{
	lst->last_exit_code = 0;
	lst->session_user = get_cwd_path_since_home();
	if (!lst->session_user)
		return (false);
	lst->session_user = getenv("USER");
	lst->git_branch_name = fetch_git_cwd_branch_name();
	if (!lst->git_branch_name)
		return (false);
	lst->time = fetch_current_time();
	if (!lst->time)
		return (false);
	lst->cwd = get_cwd_path_since_home();
	if (!lst->cwd)
		return (false);
	lst->width_without_mid_delim = ft_intlen(lst->last_exit_code);
	lst->width_without_mid_delim += ft_strlen(lst->session_user);
	lst->width_without_mid_delim += ft_strlen(lst->git_branch_name);
	lst->width_without_mid_delim += ft_strlen(lst->time);
	lst->term_width = fetch_term_width();
	if (lst->term_width == -1)
		return (false);
	return (true);
}

int	main(void)
{
	char		*res;
	int			i;
	int			type;
	t_block		*head;
	t_prompt	prompt_params;

	if (!refresh_prompt_param(&prompt_params))
		return (1);
	res = NULL;
	i = 0;
	type = -1;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	readline(build_prompt(prompt_params));
	parse_cmd(&head, rl_line_buffer);
	free(rl_prompt);
	printf("[%d] res: '%s'\n", type, res);
}
