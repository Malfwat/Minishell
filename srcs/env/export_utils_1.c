/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 00:59:45 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 01:01:14 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <libft.h>

bool	is_valid_identifier(char *name)
{
	int	i;

	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	else
	{
		i = -1;
		while (name[++i])
		{
			if (!ft_isalnum(name[i]) && name[i] != '_')
				return (false);
		}
	}
	return (true);
}
