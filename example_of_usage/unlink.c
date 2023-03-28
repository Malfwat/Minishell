/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlink.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 17:40:26 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 18:10:26 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

// Lien symbolique : Fichier qui contient le chemin vers un autre fichier
// Hard link : Fichier qui contient l'adresse physique vers un autre fichier

// Si le chemin donne a unlink est un symlink, le symlink est supprime mais pas
// ce vers quoi le symlink pointait

// Si le chemin donne est un hard link, supprime le hard link, pareil

// Si on supprime un hard link, et que c'etait le dernier a pointe vers le ficher,
// Alors le fichier est effectivement supprime (si il est ouvert, on att qu'il soit ferme)

int	main(void)
{
	if (unlink("test_files/unlink_symlink_test.txt") == -1)
	{
		perror("unlink");
		return (1);
	}
	else
		printf("Link removed\n");
	return (0);
}
