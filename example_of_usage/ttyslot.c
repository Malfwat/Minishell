/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttyslot.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:01:16 by hateisse          #+#    #+#             */
/*   Updated: 2023/03/29 20:43:27 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int main(void) {
    // Get the number of the current process's terminal device
    int tty_number = ttyslot();

	sleep(5);
    if (tty_number == 0) {
        printf("Error: no controlling terminal attached.\n");
        return 1;
    }

    // Print the number of the terminal device
    printf("Terminal device number: %d\n", tty_number);

    return 0;
}
