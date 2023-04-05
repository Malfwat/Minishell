/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:25:00 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/05 14:27:59 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <struct_ms.h>
#include <parsing.h>

int	main(void)
{
	t_block	*head;

	head = NULL;
	return (parse_cmd(&head, "ecse sef sef && sfs | sef"));
}