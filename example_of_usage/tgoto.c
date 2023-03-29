/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tgoto.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 20:08:37 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 20:17:06 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curses.h>
#include <stdio.h>
#include <term.h>

// tgoto en gros va juste remplacer les %d par les deux parametre qu'on lui donne
// l'avantage c'est que goto peut aussi reecrire toute la seq de controle si
// il detecte que la seq donne n'est pas adapte pour le terminal actuel. Il va
// Donc la reconstruire pour nous.

int	main(void)
{
	char	*ctrl_seq;
	int		y;
	int		x;
	int		i;

	i = -1;
	printf("B: ");
	while ("\033[%d;%dH"[++i])
	{
		if ("\033[%d;%dH"[i] == '\033')
			printf("\\033");
		else
			printf("%c", "\033[%d;%dH"[i]);
	}
	printf("\n");
	y = 10;
	x = 5;
	ctrl_seq = tgoto("\033[%d;%dH", y, x);
	i = -1;
	printf("A: ");
	while (ctrl_seq[++i])
	{
		if (ctrl_seq[i] == '\033')
			printf("\\033");
		else
			printf("%c", ctrl_seq[i]);
	}
}
