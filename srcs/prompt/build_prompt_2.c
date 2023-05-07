/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:26:34 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 23:54:45 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <ms_define.h>

void	build_prompt_user(t_prompt_blocks **pargs, t_prompt *params)
{
	char	*str;

	str = ft_strsjoin(9, R_ESC(LLGREY"╰─"), "\2\3", R_ESC(ITALIC), \
	R_ESC(LGREY), " ", params->session_user, R_ESC(DGREEN), " $ ", \
	R_ESC(ENDC));
	ls_p_args_addback(pargs, ls_new_p_args(P_USER, str, 0));
}

void	build_prompt_start_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(5, LLGREY, "╭─", LGREY, "░▒▓", ENDC);
	ls_p_args_addback(pargs, ls_new_p_args(P_START_DELIM, str, 0));
}
