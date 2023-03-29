/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opendir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:12:07 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 16:41:29 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

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
	dir_entry = readdir(dirp);
	while (dir_entry)
	{
		printf("Nom du fichier: %s\n", dir_entry->d_name);
		dir_entry = readdir(dirp);
	}
	if (errno)
	{
		perror("readdir");
	}
	// On ferme le flux vers le repertoire pointe par dirp
	if (closedir(dirp) == -1)
	{
		perror("closedir");
		return (1);
	}
	return (0);
}
