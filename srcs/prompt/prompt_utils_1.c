/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:07:16 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 02:10:53 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <prompt.h>

void	ft_memset_uni(char *dest, char *c, int len)
{
	while (len--)
	{
		*(dest++) = c[0];
		*(dest++) = c[1];
		*(dest++) = c[2];
	}
}

int	fetch_term_width(void)
{
	struct winsize	w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		return (-1);
	return (w.ws_col);
}

char	*get_cwd_path_since_home(void)
{
	char	*home_path;
	char	*cwd;
	char	*home_in_cwd;
	char	*result;

	result = NULL;
	home_path = getenv("HOME");
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	else if (home_path && *home_path)
	{
		home_in_cwd = ft_strnstr(cwd, home_path, ft_strlen(home_path));
		if (home_in_cwd)
		{
			result = ft_strjoin("~", cwd + ft_strlen(home_path));
			return (free(cwd), result);
		}
	}
	return (cwd);
}
