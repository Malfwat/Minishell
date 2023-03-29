/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:28:34 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 17:30:21 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

// recupere le contenu d'une variable d'environnement

int	main(void)
{
	char	*path;

	path = getenv("PATH");
	printf("PATH contient : %s\n", path);
	return (0);
}
