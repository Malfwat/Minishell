/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 01:38:43 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 01:38:54 by malfwa           ###   ########.fr       */
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
		printf("Le repertoire actuel est %s\n", current_dir);
	else
	{
		perror("getcwd");
		exit(1);
	}
	free(current_dir);
}
