/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 23:17:37 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/11 22:05:10 by hateisse         ###   ########.fr       */
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
		execve("fefe", (char *[]){"fefe", NULL}, NULL);
			perror("");
		exit(2);
	}
	else
	{
		sleep(2);
		waitpid(pid, &status, 0);
		waitpid(pid, &status, 0);
		if (errno)
			perror("");
	}
	return (0);
}
