/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:59:39 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/31 19:09:59 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <libft.h>

int	find_closing_parenthesis(char *str)
{
	int counter;
	int i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(')
			counter += 1;
		else if (str[i] == ')')
			counter -= 1;
		if (counter == 0)
			return (i - 1);
		i += 1;
	}
	return (-1);
}

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	slen;
// 	char	*res;

// 	slen = ft_strlen(s);
// 	if (slen < start)
// 		return (ft_calloc(1, sizeof(*res)));
// 	slen = ft_strlen(&s[start]);
// 	if (len < slen)
// 		slen = len;
// 	res = ft_calloc(slen + 1, sizeof(*res));
// 	if (!res)
// 		return (NULL);
// 	ft_memcpy(res, &s[start], slen);
// 	return (res);
// }


int main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	printf("len until ')': %i\n", find_closing_parenthesis(av[1]));
	printf("'%s'\n", ft_substr(&av[1][0/* i */ + 1], 0, \
	find_closing_parenthesis(av[1])));
	return (0);
}