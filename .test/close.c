/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 23:17:37 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/28 00:22:45 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int	main(void)
{
	int	pid;
	int	status;

	pid = fork();
	if (!pid)
	{
		execve("a.out", (char *[]){"a.out", "draft", NULL}, NULL);
			perror("");
		exit(2);
	}
	waitpid(pid, &status, 0);
	return (0);
}
