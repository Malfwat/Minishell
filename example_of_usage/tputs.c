/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   tputs.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hateisse <hateisse@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/03/29 18:16:57 by hateisse		  #+#	#+#			 */
/*   Updated: 2023/03/29 18:19:41 by hateisse		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <stdio.h>
#include <curses.h>

// tputs sert a envoyer une sequence de control au terminal
// exemple "\033[31m" pour que le text qui suit soit afficher en rouge
// on pourrait juste envoyer cette sequence de control avec un ft_putchar
// l'avantage de tputs est que c'est juste plus opti, et gere tout seul
// les potentiels erreurs
// Il existe des seq de control pour bouger le curseur, clear le terminal etc

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (0);
}

int	main(void)
{
	write(1, "test", 4);
	tputs("\033[31m", 3, ft_putchar);
	write(1, "rouge\nrouge", 11);
	tputs("\033[0m", 3, ft_putchar);
	write(1, "rouge\nrouge", 11);
	return (0);
}
