/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 23:10:35 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:17:01 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_prompt.h>
#include <libft.h>
#include <ms_define.h>
#include <stdio.h>

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

void	get_pargs_len(char c, int *total, int *i, bool *control_sequence)
{
	if (c == '\033')
		(*control_sequence) = true;
	else if ((*control_sequence) && c == 'm')
		(*control_sequence) = false;
	else if (!ft_isprint(c))
	{
		(*total)++;
		*i += 2;
	}
	else if (!(*control_sequence))
		(*total)++;
}

void	ls_edit_p_args_if(t_prompt_blocks *parg, int type, char *str, int dlen)
{
	while (parg)
	{
		if (parg->type == type)
		{
			free(parg->str);
			parg->str = str;
			parg->delim_len = dlen;
		}
		parg = parg->next;
	}
}

int	pargs_len(t_prompt_blocks *pargs)
{
	int		total;
	int		i;
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
				get_pargs_len(pargs->str[i], &total, &i, &control_sequence);
		}
		pargs = pargs->next;
	}
	return (total);
}
