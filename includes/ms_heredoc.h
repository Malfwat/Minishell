/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 23:18:56 by hateisse          #+#    #+#             */
/*   Updated: 2023/05/07 23:23:46 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_HEREDOC_H
# define MS_HEREDOC_H

# include <ms_struct.h>
# include <ms_define.h>

int		hd_manager(t_block *block);
int		heredoc(char *limiter);
void	ms_hd_gnl(t_fd fd, char **user_input);

#endif /* HEREDOC_H */