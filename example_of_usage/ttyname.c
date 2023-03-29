/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttyname.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:03:18 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 17:06:12 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int	main(void)
{
	int		fd;
	char	*name;

	// on test avec l'entree standard qui est un terminal
	fd = 0;
	name = ttyname(fd);
	if (name == NULL)
		perror("ttyname");
	else
		printf("fd is associated with terminal %s\n", name);
	
	// on test avec un fd d'un fichier quelconque
	fd = open("stat.c", O_RDWR);
	name = ttyname(fd);
	if (name == NULL)
		perror("ttyname");
	else
		printf("fd is associated with terminal %s\n", name);
	close(fd);
	return (0);
}
