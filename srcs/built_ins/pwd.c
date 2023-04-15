/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:38:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 15:44:32 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	pwd(void)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (current_dir)
		printf("%s\n", current_dir);
	else
	{
		perror("pwd");
		exit(0);
	}
	free(current_dir);
}
