/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstat.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:30:22 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 17:44:14 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

// Identique a stat.c sauf que si le chemin specifie est un lien symbolique
// Alors stat le lien symbolique et non le fichier auquel le lien refere.

int	main(void)
{
	struct stat	buf;

	// On genere et stock dans la structure buf
	// les informations sur le fichier
	if (lstat("test_files/lstat_symlink_test.txt", &buf) == -1)
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
