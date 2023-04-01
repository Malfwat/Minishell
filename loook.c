/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loook.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 18:32:19 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/01 18:41:23 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
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
			return (i);
		i += 1;
	}
	return (-1);
}

void	ft_error(void)
{
	return ;
}


bool 	check_io_param(char *str, int *i, char **new_line)
{
	
}

bool 	check_parenthesis_param(char *str, int *i, char **new_line)
{
	int start;

	start = *i;
	if (str[*i] == '(')
	{
		*i += find_closing_parenthesis(&str[*i]);
		if (*i == -1)
			return (ft_error(), false);
		*new_line = ft_substr(str, start, *i- start + 1);
		return (true);
		*i += 1;
	}
	return (false);
}

int	main(void)
{
	int		i;
	char	*res;
	char	*str = "012(hellc g t) what ?";

	i = 3;
	res = check_parenthesis_param(str, &i);
	printf("res = '%s' i = %d c = %c", res, i, str[i]);
	return (0);
}

// int	main(void)
// {
// 	long int i = 5;
// 	int str[19];
// 	while (i < 9)
// 		i++;
// 	printf("%li\n", (((long)&str[i] - (long)str)) / sizeof(int));
// 	return (0);
// }
