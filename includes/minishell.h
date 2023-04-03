/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 14:59:07 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/03 20:54:20 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define SINGLE_DELIM " \t><|;"

enum e_operator
{
    AND_OPERATOR,
    OR_OPERATOR,
    PIPE_OPERATOR,
    SEMI_COLON
};

#endif /* MINISHELL_H */