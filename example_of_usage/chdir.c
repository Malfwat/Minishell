/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:32:50 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/28 16:57:48 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	print_cwd(void)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (current_dir)
		printf("Le repertoire actuel est %s\n", current_dir);
	else
	{
		perror("getcwd");
		exit(1);
	}
	free(current_dir);
}

int	main(void)
{
	// on affiche le repertoire actuel (voir getcwd.c)
	print_cwd();
	// on se deplace dans le repertoire parent
	if (chdir("..") == -1)
	{
		perror("chdir");
		return (1);
	}
	else
		print_cwd(); // on affiche le nouveau repertoire
	return (0);
}
