/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:25:00 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/05 15:49:19 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <struct_ms.h>
#include <parsing_ms.h>

int	main(void)
{
	t_block	*head;

	head = NULL;
	return (parse_cmd(&head, ft_strdup("ok ls && ls")));
}
