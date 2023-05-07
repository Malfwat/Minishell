/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:26:34 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/08 00:23:55 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <ms_define.h>

void	build_prompt_user(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(9, "\001"LLGREY"╰─""\002", "\2\3", "\001"ITALIC"\002", \
	"\001"LGREY"\002", " ", params->session_user, "\001"DGREEN"\002", " $ ", \
	"\001"ENDC"\002");
	ls_p_args_addback(pargs, ls_new_p_args(P_USER, str, 0));
}

void	build_prompt_start_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(5, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_START_DELIM, str, 0));
}
