/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 03:14:25 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/10 19:57:10 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HISTORY_H
# define HISTORY_H

int		get_my_history(void);
void	my_add_history(char *str, int fd);

#endif	/* HISTORY_H */