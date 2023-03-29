/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_history_and_rl_clear_history.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:40:32 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/29 03:41:43 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    while (1)
    {
        readline(">>");
        // rl_redisplay();
        add_history(rl_line_buffer);
    }
    free(rl_line_buffer);
    rl_clear_history();
    return (0);
}

/*
Add_history will simply keep in memory each line got by the usage of readline
When readline wait for an input we can access to the history
*/

/*
rl_clear_history free the history
*/
