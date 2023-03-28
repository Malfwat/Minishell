/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fstat.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:25:23 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 17:30:56 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

// Identique a stat.c sauf qu'on specifie un fd et non un nom de fichier

int	main(void)
{
	struct stat	buf;
	int			fd;

	fd = open("stat_test_file.txt", O_RDONLY);
	// On genere et stock dans la structure buf
	// les informations sur le fichier
	if (fstat(fd, &buf) == -1)
	{
		perror("stat");
		return (1);
	}
	printf("\
		Last access time: %ld\n \
		Last modification time: %ld\n \
		Last state modification time: %ld\n \
		Total size in Byte: %ld\n \
		Hard links count: %ld\n \
		Owner UID: %u\n \
		Group owner UID: %u\n", \
		buf.st_atime, buf.st_mtime, buf.st_ctime, buf.st_size, \
		buf.st_nlink, buf.st_uid, buf.st_gid);
	close(fd);
	return (0);
}
