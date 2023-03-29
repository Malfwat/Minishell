/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:12:07 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 18:57:15 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

// Ouvre un repertoire. A utilise avec readdir ou autre fonction relate

int	main(void)
{
	DIR				*dirp;
	struct dirent	*dir_entry;

	// On ouvre un flux vers le repertoire courant
	dirp = opendir(".");
	if (!dirp)
	{
		perror("dirp");
		return (1);
	}
	// On accede aux element du repertoire, un par un
	// on set errno a 0 pour distinguer une erreur de la fin de la liste
	errno = 0;
	while (dir_entry)
	{
		dir_entry = readdir(dirp);
		printf()
	}
	if (errno)
	{
		perror()
	}
	// On ferme le flux vers le repertoire pointe par dirp
	if (closedir(dirp) == -1)
	{
		perror("closedir");
		return (1);
	}
}
