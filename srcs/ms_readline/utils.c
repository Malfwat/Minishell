/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 13:38:58 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 13:59:30 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_readline.h>
#include <libft.h>

char	check_for_quotes(char *str, char quote)
{
	char	*closing_quote;
	char	*quote_found;

	if (!str || !*str)
		return (quote);
	quote_found = ft_strchr_set(str, "'\"");
	if (quote && quote_found && *quote_found == quote)
		return (check_for_quotes(quote_found + 1, 0));
	else if (quote_found && !quote)
	{
		closing_quote = ft_strchr(quote_found + 1, *quote_found);
		if (closing_quote)
			return (check_for_quotes(closing_quote + 1, 0));
		return (*quote_found);
	}
	return (quote);
}

void	update_quotes(char *str, char *quotes)
{
	*quotes = check_for_quotes(str, *quotes);
}
