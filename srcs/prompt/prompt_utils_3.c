/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:20:31 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 20:47:03 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <ms_define.h>
#include <stdio.h>

void	build_upper_prompt(t_prompt *params, t_prompt_blocks **pargs)
{		
	build_prompt_start_delim(pargs);
	build_prompt_cwd(pargs, params);
	if (params->git_branch_name)
		build_prompt_git(pargs, params);
	build_prompt_mid_delim(pargs, 10);
	build_prompt_exit_status(pargs, params);
	build_prompt_time(pargs, params);
	check_prompt_width(*pargs, params);
	build_prompt_end_delim(pargs);
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
	ls_edit_p_args_if(pargs, P_MID_DELIM, build_mid_delim(new_delim_mlen), \
	new_delim_mlen);
}

int	current_mid_delim_len(t_prompt_blocks *pargs)
{
	while (pargs)
	{
		if (pargs->type == P_MID_DELIM)
			return (pargs->delim_len);
		pargs = pargs->next;
	}
	return (0);
}

bool	refresh_prompt_param(t_prompt *lst, int last_exit_code)
{
	lst->last_exit_code = last_exit_code;
	lst->session_user = getenv("USER");
	lst->git_branch_name = fetch_git_cwd_branch_name();
	lst->time = fetch_current_time();
	lst->cwd = get_cwd_path_since_home();
	lst->term_width = fetch_term_width();
	errno = 0;
	return (true);
}

int	extract_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) | 0x80);
	return (0);
}
