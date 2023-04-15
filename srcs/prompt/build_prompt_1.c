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

void	build_prompt_exit_status(char **prompt, t_prompt *params)
{
	char	*tmp;
	char	*ascii_status;

	ascii_status = ft_itoa(params->last_exit_code);
	if (!ascii_status)
		return ;
	tmp = *prompt;
	if (params->last_exit_code == 0)
		*prompt = ft_strsjoin(7, tmp, LGREY_BG, GREEN, "✔ ", LLGREY, "", ENDC);
	else
		*prompt = ft_strsjoin(8, tmp, RED, LGREY_BG, ascii_status, " ✘ ",
				LLGREY, "", ENDC);
	free(ascii_status);
	free(tmp);
}

bool	build_prompt_mid_delim(char **prompt, t_prompt *params)
{
	char	*tmp;
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

void	build_prompt_end_delim(char **prompt)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(9, tmp, ENDC, LGREY, "▓▒░", ENDC, LLGREY, "\n╰─",
			BOLD, ENDC);
	free(tmp);
}

char	*build_prompt(t_prompt *params)
{
	char	*prompt;

	errno = 0;
	prompt = ft_calloc(1, 1);
	if (prompt)
	{
		build_prompt_start_delim(&prompt);
		build_prompt_cwd(&prompt, params);
		if (params->git_branch_name)
			build_prompt_git(&prompt, params);
		build_prompt_mid_delim(&prompt, params);
		build_prompt_exit_status(&prompt, params);
		build_prompt_time(&prompt, params);
		build_prompt_end_delim(&prompt);
		build_prompt_user(&prompt, params);
	}
	if (errno)
		return (free(prompt), NULL);
	return (free_prompt_params(params), prompt);
}

bool	refresh_prompt_param(t_prompt *lst)
{
	lst->last_exit_code = 0;
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
	lst->width_without_mid_delim = ft_intlen(lst->last_exit_code);
	lst->width_without_mid_delim += ft_strlen(lst->session_user);
	if (lst->git_branch_name)
		lst->width_without_mid_delim += ft_strlen(lst->git_branch_name);
	lst->width_without_mid_delim += ft_strlen(lst->time);
	lst->term_width = fetch_term_width();
	if (lst->term_width == -1)
		return (false);
	return (true);
}
