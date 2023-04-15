/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:45:02 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/12 17:10:16 by hateisse         ###   ########.fr       */
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

t_prompt_blocks	*ls_new_p_args(char type, char *str)
{
	t_prompt_blocks	*new;

	new = ft_calloc(1, sizeof(t_prompt_blocks));
	if (!new)
		return (free(str), NULL);
	new->type = type;
	new->str = str;
	return (new);
}

void	build_prompt_time(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(6, LGREY_BG, " ", LBLUE, params->time, LLGREY, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(1, str));
}

void	build_prompt_git(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(10, LGREY_BG, LLGREY, \
			"  ", ENDC, LGREY_BG, "\u2387  ", LGREEN, \
			params->git_branch_name, LLGREY, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(2, str));
}

void	build_prompt_cwd(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(6, BOLD, LGREY_BG, " ", LCYAN, params->cwd, ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(3, str));
}

void	build_prompt_start_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(5, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(4, str));
}

void	build_prompt_user(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(7, ITALIC, LGREY, " ", \
	params->session_user, DGREEN, " $ ", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(5, str));
}
