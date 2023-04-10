/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:45:02 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/10 20:08:05 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prompt.h>
#include <libft.h>
#include <ms_define.h>

void	build_prompt_time(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, LGREY_BG, " ", LBLUE, params.time, \
	LLGREY, ENDC);
	free(tmp);
	free(params.time);
}

void	build_prompt_git(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(11, tmp, LGREY_BG, LLGREY, \
			"  ", ENDC, LGREY_BG, "\u2387  ", LGREEN, \
			params.git_branch_name, LLGREY, ENDC);
	free(tmp);
	free(params.git_branch_name);
}

void	build_prompt_cwd(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(7, tmp, BOLD, LGREY_BG, " ", LCYAN, \
	params.cwd, ENDC);
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

void	build_prompt_user(char **prompt, t_prompt params)
{
	char	*tmp;

	tmp = *prompt;
	*prompt = ft_strsjoin(8, tmp, ITALIC, LGREY, " ", \
	params.session_user, DGREEN, " $ ", ENDC);
	free(tmp);
}
