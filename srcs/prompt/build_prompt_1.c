/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_prompt_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 01:47:15 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:11:56 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <ms_define.h>
#include <stdio.h>

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
		len = 5;
	str = build_mid_delim(len);
	ls_p_args_addback(pargs, ls_new_p_args(P_MID_DELIM, str, len));
}

void	build_prompt_end_delim(t_prompt_blocks **pargs)
{
	char	*str;

	str = ft_strsjoin(5, ENDC, LGREY, "▓▒░", ENDC, "\n");
	ls_p_args_addback(pargs, ls_new_p_args(P_END_DELIM, str, 0));
}

char	*build_prompt(t_prompt *params, bool side)
{
	t_prompt_blocks	*pargs;
	char			*prompt;

	errno = 0;
	(void)side;
	pargs = NULL;
	build_upper_prompt(params, &pargs);
	build_prompt_user(&pargs, params);
	prompt = strjoin_pargs(pargs);
	ls_free_pargs(pargs);
	if (errno)
		return (free(prompt), NULL);
	return (prompt);
}
