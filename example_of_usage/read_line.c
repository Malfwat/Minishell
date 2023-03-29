/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:13:04 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/29 14:39:40 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

int	main(void)
{
	readline("This is my prompt: ")
	printf("On a un prompt: %s et une ligne obtenue %s \n", rl_prompt, rl_line_buffer);
	free(rl_line_buffer);
	free(rl_prompt);
	return (0);
}

/*
Here readline(char *prompt) will display a prompt and wait for the user to write something
when it's done the commande line will be stored in the variable rl_line_buffer and prompt will be stored in rl_prompt
Both need to be freed
*/