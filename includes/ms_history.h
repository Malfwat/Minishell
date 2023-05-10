/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 03:14:25 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:05:00 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_HISTORY_H
# define MS_HISTORY_H

# include <ms_struct.h>

int		get_my_history(void);
void	ms_add_history(char *str);

#endif /* HISTORY_H */