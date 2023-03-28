/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getcwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:15:30 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 16:34:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*current_dir;

	// On appel getcwd pour obtenir le repertoire dans lequel on est.
	// On lui donne NULL et 0 en parametre pour qu'il puisse malloc la taille necessaire
	current_dir = getcwd(NULL, 0);
	// Si le retour est NULL il y a eu une erreur.
	if (!current_dir)
		perror("getcwd");
	else
		printf("%s\n", current_dir);
	free(current_dir);
	return (0);
}
