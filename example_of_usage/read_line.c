/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:13:04 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/11 21:27:12 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char *ptr = readline("This is my prompt: ");
	printf("ptr: %p rl_li.. %p\n", ptr, rl_line_buffer);
	printf("ptr: '%s' rl_li.. '%s'\n", ptr, rl_line_buffer);
	free(rl_line_buffer);
	free(rl_prompt);
	return (0);
}

/*
Here readline(char *prompt) will display a prompt and wait for the user to write something
when it's done the commande line will be stored in the variable rl_line_buffer and prompt will be stored in rl_prompt
Both need to be freed
*/