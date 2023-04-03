/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:53:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <struct.h>

int	cd(t_env_var	*head, char *str)
{
	if (chdir(str) == -1)
	{
		perror("cd");
		return (1);
	}
	export(&head, ft_strjoin(ft_strdup("PWD="), getcwd(NULL, 0)));
	return (0);
}
