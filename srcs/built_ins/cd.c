/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:33:59 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 16:53:58 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <env_function.h>
#include <libft.h>
#include <struct_ms.h>

int	cd(t_env_var *head, char *str)
{
	if (!str)
		str = getenv("HOME");
	if (chdir(str) == -1)
	{
		perror("cd");
		return (1);
	}
	export(&head, ft_strdup("PWD="), getcwd(NULL, 0), 0);
	return (0);
}
