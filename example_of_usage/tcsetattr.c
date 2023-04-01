/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcsetattr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:46:00 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/29 18:20:18 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <termios.h>
#include <unistd.h>

int main() {
    // Set up terminal attributes for non-canonical input
    struct termios original_term, new_term;
    tcgetattr(STDIN_FILENO, &original_term);
    memcpy(&new_term, &original_term, sizeof(struct termios));
    new_term.c_lflag &= ~(ECHO);
    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    tgetflag("us");

    // Read and process input character by character
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        // Check for specific keys or characters and process accordingly
        if (c == '\n') {
            // Newline character, end of line
            printf("\n");
        } else if (c == 127) {
            // Backspace key, erase last character
            printf("\b \b");
        } else if (c >= ' ' && c <= '~') {
            // Printable character, print it
            printf("%c", c);
        } else if (c == '6')
            break ;
    }

    // Restore original terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);

    return 0;
}
