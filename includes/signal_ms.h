/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_ms.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:49:51 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/07 04:50:57 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_MS_H
# define SIGNAL_MS_H

void	sigquit_handler(int num);
void	set_sig_handler(void);

#endif /* SIGNAL_MS_H */