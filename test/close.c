/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 23:17:37 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/10 23:23:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	int fd = open("adam", O_RDONLY);
	int	tube[2];

	pipe(tube);
	close(tube[1]);
	dup2(tube[0], 0);
	perror("");
}