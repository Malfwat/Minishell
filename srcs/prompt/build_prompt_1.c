/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:47:15 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/15 23:39:39 by hateisse         ###   ########.fr       */
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
		return (WTERMSIG(status) | 0x80);
	return (0);
}

char	*build_mid_delim(int len)
{
	char	*delim;
	char	*str;
	delim = NULL;
	if (len < 5)
		len = 5;
	if (len > 0)
	{
		delim = ft_calloc(len + 1, sizeof(char) * 3);
		if (!delim)
			return (NULL);
		ft_memset_uni(delim, "─", len);
	}
	str = ft_strsjoin(7, LGREY, "▓▒░", LLGREY, delim,
			LGREY, "░▒▓", ENDC);
	free(delim);
	return (str);
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
	ls_p_args_addback(pargs, ls_new_p_args(P_EXIT_STATUS, str, 0));
	free(ascii_status);
}

void	build_prompt_mid_delim(t_prompt_blocks **pargs, int len)
{
	char	*str;

	if (len < 5)
		len = 5; // minimum width of mid delim
	// len = params->term_width - params->width_without_mid_delim - offset;
	str = build_mid_delim(len);
	ls_p_args_addback(pargs, ls_new_p_args(P_MID_DELIM, str, len));
}

void	build_prompt_end_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(8, ENDC, LGREY, "▓▒░", ENDC, LLGREY, "\n╰─",
			BOLD, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_END_DELIM, str, 0));
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
		pargs = pargs->next;
	}
	return (str);
}

int		pargs_len(t_prompt_blocks *pargs)
{
	int	total;
	int	i;
	bool	control_sequence;

	control_sequence = true;
	total = 4;
	while (pargs)
	{
		i = -1;
		if (pargs->type != P_USER && pargs->type != P_END_DELIM)
		{
			if (pargs->type == P_GIT && pargs->str)
				total += 1;
			while (pargs->str[++i])
			{
				if (pargs->str[i] == '\033')
					control_sequence = true;
				else if (control_sequence && pargs->str[i] == 'm')
					control_sequence = false;
				else if (!ft_isprint(pargs->str[i]))
				{
					total++; 
					i += 2;
				}
				else if (!control_sequence)
					total++; 
			}
		}
		pargs = pargs->next;
	}
	return (total);
}

void	ls_edit_p_args_if(t_prompt_blocks *pargs, int type, char *str, int delim_len)
{
	while (pargs)
	{
		if (pargs->type == type)
		{
			free(pargs->str);
			pargs->str = str;
			pargs->delim_len = delim_len;
		}
		pargs = pargs->next;
	}
}

int		current_mid_delim_len(t_prompt_blocks *pargs)
{
	while (pargs)
	{
		if (pargs->type == P_MID_DELIM)
			return (pargs->delim_len);
		pargs = pargs->next;
	}
	return (0);
}

void	check_prompt_width(t_prompt_blocks *pargs, t_prompt *params)
{
	int				len;
	int				new_delim_mlen;
	int				current_delim_mlen;

	len = pargs_len(pargs);
	current_delim_mlen = current_mid_delim_len(pargs);
	if (len > params->term_width)
		new_delim_mlen = current_delim_mlen - (len - params->term_width);
	else if (len < params->term_width)
		new_delim_mlen = current_delim_mlen + (params->term_width - len);
	else
		return ;
	ls_edit_p_args_if(pargs, P_MID_DELIM, build_mid_delim(new_delim_mlen), new_delim_mlen);
}

char	*build_prompt(t_prompt *params)
{
	t_prompt_blocks	*pargs;
	char			*prompt;

	errno = 0;
	pargs = NULL;

	build_prompt_start_delim(&pargs);
	build_prompt_cwd(&pargs, params);
	if (params->git_branch_name)
		build_prompt_git(&pargs, params);
	build_prompt_mid_delim(&pargs, 10);
	build_prompt_exit_status(&pargs, params);
	build_prompt_time(&pargs, params);
	build_prompt_end_delim(&pargs);
	build_prompt_user(&pargs, params);
	check_prompt_width(pargs, params);
	prompt = strjoin_pargs(pargs);
	ls_free_pargs(pargs);
	free_prompt_params(params);
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
	lst->term_width = fetch_term_width();
	if (lst->term_width == -1)
		return (false);
	return (true);
}
