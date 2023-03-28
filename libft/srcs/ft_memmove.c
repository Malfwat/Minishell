/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 15:19:07 by amouflet          #+#    #+#             */
/*   Updated: 2022/11/10 19:02:42 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	c;
	size_t	i;

	i = 0;
	while (((long)dest) > ((long)src) && n > 0)
	{
		c = ((char *)src)[n - 1];
		((char *)dest)[n - 1] = c;
		n--;
	}
	while (((long)dest) <= ((long)src) && i < n)
	{
		c = ((char *)src)[i];
		((char *)dest)[i] = c;
		i++;
	}
	return (dest);
}
