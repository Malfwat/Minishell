/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 00:04:27 by malfwa            #+#    #+#             */
/*   Updated: 2022/11/17 13:14:55 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	x;
	char	*str;

	i = 0;
	x = 0;
	str = (char *)big;
	while (str && little && str[i + x] && x + i < len && little[x])
	{
		if (str[i + x] == little[x])
			x++;
		else
		{
			x = 0;
			i++;
		}
	}
	if (little[x] == '\0' || little == NULL || little[0] == 0)
		return (&str[i]);
	if (len == 0)
		return (0);
	return (NULL);
}
