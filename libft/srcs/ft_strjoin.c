/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:31:58 by amouflet          #+#    #+#             */
/*   Updated: 2022/11/14 02:30:47 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1 && s1[i])
		i++;
	while (s2 && s2[j])
		j++;
	str = malloc(sizeof(char) * (i + j + 1));
	if (str == NULL)
		return (str);
	str[i + j] = 0;
	i = -1;
	while (s1 && s1[++i])
		str[i] = s1[i];
	j = -1;
	while (s2 && s2[++j])
		str[i + j] = s2[j];
	return (str);
}
