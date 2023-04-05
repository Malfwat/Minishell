/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:12:21 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/05 21:34:39 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <libft.h>
#include <parsing_ms.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

void	build_prompt_exit_status(char **prompt, int status)
{
	char	*tmp;
	char	*ascii_status;

	ascii_status = ft_itoa(status);
	if (!ascii_status)
		return ;
	tmp = *prompt;
	if (status == 0)
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
		if (close(tube[0]) == -1)
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

void	build_prompt_time(char **prompt)
{
	char	*tmp;
	char	*time;

	time = fetch_current_time();
	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, LGREY_BG, " ", LBLUE, time, LLGREY, ENDC);
	free(tmp);
	free(time);
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
		if (close(tube[0]) == -1)
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
	if (*res && res[ft_strlen(res) - 1] == '\n')
		res[ft_strlen(res) - 1] = 0;
	return (res);
}

void	build_prompt_git(char **prompt)
{
	char	*tmp;
	char	*branch;

	branch = fetch_git_cwd_branch_name();
	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, LGREY_BG, "\u2387  ", LGREEN, branch, LLGREY, ENDC);
	free(tmp);
	free(branch);
}

void	build_prompt_cwd(char **prompt, char *cwd)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(9, tmp, BOLD, LGREY_BG, " ", LCYAN, cwd, LLGREY,
			"  ", ENDC);
	free(tmp);
}

void	build_prompt_start_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(5, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	free(tmp);
}

void	build_prompt_mid_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(8, tmp, LGREY, "▓▒░", LLGREY, "─────────────────",
			LGREY, "░▒▓", ENDC);
	free(tmp);
}

void	build_prompt_end_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(12, tmp, ENDC, LGREY, "▓▒░", ENDC, LLGREY, "\n╰─",
			BOLD, ENDC, DGREEN, " $ ", ENDC);
	free(tmp);
}

char	*build_prompt(int status, char *cwd)
{
	char	*prompt;

	errno = 0;
	prompt = ft_calloc(1, 1);
	if (prompt)
	{
		build_prompt_start_delim(&prompt);
		build_prompt_cwd(&prompt, cwd);
		build_prompt_git(&prompt);
		build_prompt_mid_delim(&prompt);
		build_prompt_exit_status(&prompt, status);
		build_prompt_time(&prompt);
		build_prompt_end_delim(&prompt);
	}
	if (errno)
		return (free(prompt), NULL);
	return (prompt);
}

int	main(void)
{
	char	*res;
	int		i;
	int		type;
	t_block	*head;

	res = NULL;
	i = 0;
	type = -1;
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (perror("minishell"), 1);
	readline(build_prompt(125, "~/42-CURSUS/Minishell"));
	parse_cmd(&head, rl_line_buffer);
	free(rl_prompt);
	printf("[%d] res: '%s'\n", type, res);
}
