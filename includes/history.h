/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 03:14:25 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 03:15:48 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HISTORY_H
# define HISTORY_H

int		get_my_history(void);
void	my_add_history(char *str, int fd);

#endif	/* HISTORY_H */