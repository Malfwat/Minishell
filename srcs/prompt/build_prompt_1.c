/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:47:15 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 17:07:16 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prompt.h>
#include <libft.h>
#include <ms_define.h>
#include <stdio.h>

int	extract_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	return (0);
}

void	build_prompt_exit_status(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;
	char	*ascii_status;

	ascii_status = ft_itoa(extract_exit_code(params->last_exit_code));
	if (!ascii_status)
		return ;
	if (!params->last_exit_code)
		str = ft_strsjoin(6, LGREY_BG, GREEN, "✔ ", LLGREY, "", ENDC);
	else
		str = ft_strsjoin(7, RED, LGREY_BG, ascii_status, " ✘ ", LLGREY,
				"", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(8, str));
	free(ascii_status);
}

void	build_prompt_mid_delim(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;
	char	*delim;
	int		len;

	len = params->term_width - params->width_without_mid_delim - 37;
	if (!params->git_branch_name)
		len += 23;
	delim = NULL;
	if (len > 0)
	{
		delim = ft_calloc(len + 1, sizeof(char) * 3);
		if (!delim)
			return ;
		ft_memset_uni(delim, "─", len);
	}
	str = ft_strsjoin(7, LGREY, "▓▒░", LLGREY, delim,
			LGREY, "░▒▓", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(7, str));
	free(delim);
}

void	build_prompt_end_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(8, ENDC, LGREY, "▓▒░", ENDC, LLGREY, "\n╰─",
			BOLD, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(6, str));
}

void	ls_free_pargs(t_prompt_blocks *pargs)
{
	t_prompt_blocks	*tmp;

	while (pargs)
	{
		tmp = pargs->next;
		free(pargs->str);
		free(pargs);
		pargs = tmp;
	}
}

char	*strjoin_pargs(t_prompt_blocks *pargs)
{
	char	*str;
	char	*tmp;

	str = NULL;
	while (pargs)
	{
		tmp = str;
		str = ft_strjoin(str, pargs->str);
		free(tmp);
		if (errno)
			return (NULL);
	}
	return (str);
}

char	*build_prompt(t_prompt *params)
{
	t_prompt_blocks	**pargs;

	errno = 0;
	pargs = NULL;

	build_prompt_start_delim(&pargs);
	build_prompt_cwd(&pargs, params);
	if (params->git_branch_name)
		build_prompt_git(&pargs, params);
	build_prompt_mid_delim(&pargs, params);
	build_prompt_exit_status(&pargs, params);
	build_prompt_time(&pargs, params);
	build_prompt_end_delim(&pargs);
	build_prompt_user(&pargs, params);
	prompt = strjoin_pargs(*pargs);
	ls_free_pargs(*pargs);
	free_prompt_params(params)
	if (errno)
		return (free(prompt), NULL);
	return (prompt);
}

bool	refresh_prompt_param(t_prompt *lst, int last_exit_code)
{
	lst->last_exit_code = last_exit_code;
	lst->session_user = getenv("USER");
	lst->git_branch_name = fetch_git_cwd_branch_name();
	if (errno)
		return (false);
	lst->time = fetch_current_time();
	if (!lst->time)
		return (false);
	lst->cwd = get_cwd_path_since_home();
	if (!lst->cwd)
		return (false);
	lst->width_without_mid_delim = ft_intlen(extract_exit_code(last_exit_code));
	lst->width_without_mid_delim += ft_strlen(lst->session_user);
	if (lst->git_branch_name)
		lst->width_without_mid_delim += ft_strlen(lst->git_branch_name);
	lst->width_without_mid_delim += ft_strlen(lst->time);
	lst->term_width = fetch_term_width();
	if (lst->term_width == -1)
		return (false);
	return (true);
}
