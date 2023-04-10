/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_ms.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:49:51 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/10 19:57:06 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_MS_H
# define SIGNAL_MS_H

void	sigquit_handler(int num);
void	set_sig_handler(void);

#endif /* SIGNAL_MS_H */