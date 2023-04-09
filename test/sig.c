/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 00:03:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/10 00:20:30 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t pid = fork();
	printf("pid=%d\n", pid);
	if (!pid)
	{
		return (0);
	}
	// wait(NULL);
	// sleep(200);
	// printf("yo %i\n", kill(pid, SIGTERM));
	return (0);
}