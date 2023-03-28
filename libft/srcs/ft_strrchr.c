/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 19:43:16 by malfwa            #+#    #+#             */
/*   Updated: 2022/11/15 10:20:49 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;
	char	*tmp;
	char	c2;

	c2 = (char)c;
	str = (char *)s;
	i = 0;
	tmp = NULL;
	while (str[i])
	{
		if (str[i] == c2)
			tmp = &(str[i]);
		i++;
	}
	if (c2 == '\0')
		tmp = &(str[i]);
	return (tmp);
}
