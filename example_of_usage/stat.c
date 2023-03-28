/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:58:24 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 17:22:13 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	struct stat	buf;

	// On genere et stock dans la structure buf
	// les informations sur le fichier
	if (stat("stat_test_file.txt", &buf) == -1)
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
	return (0);
}
