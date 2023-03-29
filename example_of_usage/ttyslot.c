/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttyslot.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:01:16 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 17:01:22 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Renvoie 

int	main(void)
{
	int	slot;

	slot = ttyslot();
	printf("%d\n", slot);
	return (0);
}
