/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:13:04 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/17 20:01:13 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <libft.h>
#include <stdlib.h>

#define R_ESC(sc) "\001"sc"\002"
#define JOIN(a, b) ""a""b


int	main(void)
{
	printf("\033[38;5;243m╭─\033[38;5;235m░▒▓\033[0m\033[1m\033[48;5;235m \033[38;5;38m~/42-CURSUS/Minishell\033[0m\033[48;5;235m\033[38;5;243m  \033[0m\033[48;5;235m⎇  \033[38;5;76mhaickel\033[38;5;243m\033[0m\033[38;5;235m▓▒░\033[38;5;243m\033[38;5;235m░▒▓\033[0m\033[48;5;235m\033[38;5;10m✔ \033[38;5;243m\033[0m\033[48;5;235m \033[38;5;110m18:54:09\033[38;5;243m\033[0m\033[0m\033[38;5;235m▓▒░\033[0m\n");
	fflush(0);
	// char *tmp = ;
	// rl_on_new_line();
	// printf("%o\n", (unsigned char)-128);
	char *ptr = readline("\001\033[38;5;243m╰─\033[3m\033[38;5;235m\002\3\3 hateisse\001\033[38;5;22m\002 $ \001\033[0m\002");
	// printf("ptr: %p rl_li.. %p\n", ptr, rl_line_buffer);
	// printf("ptr: '%s' rl_li.. '%s'\n", ptr, rl_line_buffer);
	// free(rl_line_buffer);
	// free(rl_prompt);
	// char *str = "ad";
	// printf(JOIN(str, "am"));
	// printf(("test"));
	return (0);
}

/*
Here readline(char *prompt) will display a prompt and wait for the user to write something
when it's done the commande line will be stored in the variable rl_line_buffer and prompt will be stored in rl_prompt
Both need to be freed
*/