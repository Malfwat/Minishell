/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isatty.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:00:47 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 17:04:05 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Indque si un file descriptor est un terminal.
// stdin stdout stderr le sont de base

int main(void) {
	int	fd;

	fd = open("/dev/tty0", O_RDONLY);
	// on testle fichier lstat.c
	if (isatty(fd))
		printf("is a terminal\n");
	else
		printf("is not a terminal\n");
	close(fd);

	// on test l'entree standard
	if (isatty(1))
		printf("is a terminal\n");
	else
		printf("is not a terminal\n");
	return (0);
}
