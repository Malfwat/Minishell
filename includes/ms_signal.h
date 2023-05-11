/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:49:51 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 16:51:55 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_SIGNAL_H

# define MS_SIGNAL_H

void	handler_readline(int num);
void	handler_hd_close(int num);
void	child_reset_signals(int nb, ...);

#endif /* MS_SIGNAL_H */
