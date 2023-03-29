/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_open_read_close.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:50:02 by malfwa            #+#    #+#             */
/*   Updated: 2023/03/29 15:08:58 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int ac, char **av)
{
    // First we gonn check if we have the right nunber of arg
    if (ac != 2)
        return (write(2, "./a.out file_name", 17), 0);

    // Now we gonn use access() to check if the file exist and check the rights
    if (access(av[1], F_OK) == -1)
        return (write(2, "The file doesn't exist\n", 23), 0);
    if (access(av[1], R_OK) == -1)
        return (write(2, "The file can't be read\n", 23), 0);
    if (access(av[1], W_OK) == -1)
        return (write(2, "The file can't be written\n", 26), 0);
    if (access(av[1], X_OK) == -1)
        return (write(2, "The file can't be executed\n", 27), 0);

    // Let's open the file and read the first char'
    int fd = open(av[1], O_RDONLY); // With O_RDONLY we will just read the file nothing else
    char buffer[15]; // the buffer where will be stocked the read char
    
    buffer[read(fd, buffer, 14)] = 0;

    // now we can close and print what we read 
    close(fd);
    printf("%s\n", buffer);
    return (0);
}