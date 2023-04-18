/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 03:14:25 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/18 02:07:55 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	HISTORY_H
# define HISTORY_H

int		get_my_history(t_minishell *ms_params);
void	ms_add_history(char *str, t_minishell *ms_params);

#endif	/* HISTORY_H */