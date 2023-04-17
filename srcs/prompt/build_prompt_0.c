/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:45:02 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/17 20:19:48 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <prompt.h>
#include <libft.h>
#include <ms_define.h>

void	ls_p_args_addback(t_prompt_blocks **head, t_prompt_blocks *new)
{
	t_prompt_blocks	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_prompt_blocks	*ls_new_p_args(char type, char *str, int delim_len)
{
	t_prompt_blocks	*new;

	new = ft_calloc(1, sizeof(t_prompt_blocks));
	if (!new)
		return (free(str), NULL);
	new->type = type;
	new->str = str;
	new->delim_len = delim_len;
	return (new);
}

void	build_prompt_time(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(6, LGREY_BG, " ", LBLUE, params->time, LLGREY, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_TIME, str, 0));
}

void	build_prompt_git(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(10, LGREY_BG, LLGREY, \
			"  ", ENDC, LGREY_BG, "\u2387  ", LGREEN, \
			params->git_branch_name, LLGREY, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_GIT, str, 0));
}

void	build_prompt_cwd(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(6, BOLD, LGREY_BG, " ", LCYAN, params->cwd, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_CWD, str, 0));
}

void	build_prompt_start_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(5, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_START_DELIM, str, 0));
}

void	build_prompt_user(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(9, R_ESC(LLGREY"╰─"), "\2\3", R_ESC(ITALIC), R_ESC(LGREY), " ", \
	params->session_user, R_ESC(DGREEN), " $ ", R_ESC(ENDC));
	ls_p_args_addback(pargs, ls_new_p_args(P_USER, str, 0));
}
