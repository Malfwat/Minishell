/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:49:51 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/07 13:59:10 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_READLINE_H

# define MS_READLINE_H

void	readline_child(void);
void	ms_readline(char *tmp, char quote);
void	rdl_write_in_pipe(char	*str, char quote);
void	rdl_backslash(char **last_read, char *quote);
void	error_ms_readline(const char quote);

/* utils */

char	check_for_quotes(char *str, char quote);
void	update_quotes(char *str, char *quotes);

#endif /* MS_READLINE_H */
