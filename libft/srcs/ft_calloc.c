/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:08:19 by amouflet          #+#    #+#             */
/*   Updated: 2022/11/17 16:26:26 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	max_int;
	char	*tmp;
	size_t	i;
	size_t	value_asked;

	max_int = INT_MAX;
	i = 0;
	if (nmemb == 0 || size == 0)
	{
		tmp = malloc(sizeof(char));
		if (tmp)
			tmp[0] = 0;
		return ((void *)tmp);
	}
	if (nmemb > max_int / size || size > max_int / nmemb)
		return (NULL);
	value_asked = nmemb * size;
	tmp = malloc((value_asked));
	if (tmp == NULL)
		return (NULL);
	while (i < value_asked)
		tmp[i++] = 0;
	return ((void *)tmp);
}
