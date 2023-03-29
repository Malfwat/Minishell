/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_replace_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:29:01 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/29 14:48:35 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>

int main(void)
{
	readline(">> ");
	rl_replace_line("hehehe\n\0", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* 
rl_replace_line() change the line in the variable rl_line_buffer
rl_redisplay() print rl_line_buffer if the previous line is different to it
In order to use rl_redisplay() we often need to use rl_on_new_line() so rl_redisplay() can be used correctely
*/