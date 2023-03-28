/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 19:00:57 by amouflet          #+#    #+#             */
/*   Updated: 2022/11/08 16:26:35 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, char *src, size_t size)
{
	size_t	x;
	size_t	len_dst;
	size_t	len_src;

	len_dst = 0;
	len_src = 0;
	while (dst[len_dst] != '\0')
		len_dst++;
	while (src[len_src] != '\0')
		len_src++;
	if (size == 0)
		return (len_src);
	x = 0;
	while (src[x] != '\0' && len_dst + x < size - 1)
	{
		dst[len_dst + x] = src[x];
		x++;
	}
	dst[len_dst + x] = '\0';
	if (size > len_dst)
		return (len_src + len_dst);
	return (len_src + size);
}
