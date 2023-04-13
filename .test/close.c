/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 23:17:37 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/12 20:44:44 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int	main(void)
{
	int	pid;
	int	status;

	pid = fork();
	if (!pid)
	{
		execve("(echo test)", (char *[]){"fefe", NULL}, NULL);
			perror("");
		exit(2);
	}
	waitpid(pid, &status, 0);
	return (0);
}
