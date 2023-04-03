/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:59:39 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/01 18:26:13 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <struct.h>
#include <minishell.h>
#include <libft.h>

int	is_delimiter(char *str, int i)
{
	if (str[i] == '|')
	{
		if (str[i + 1] == '|')
			return (OR_OPERATOR);
		return (PIPE_OPERATOR);
	}
	if (str[i] == '&' && str[i + 1] == '&')
		return (AND_OPERATOR);
	if (str[i] == ';')
		return (SEMI_COLON);
	return (-1);
}

// function using substr
int trouve_la_prochaine_portion_a_etudier(char *cmd_line, char **str/* a remplir avec la portion retiree */, \
int *position_dans_la_string)
{
	int i = *position_dans_la_string;
	int delimiter;

	while (cmd_line[i])
	{
		delimiter = is_delimiter(cmd_line, i);
		if (delimiter != -1)
			break;
		i++;
	}
	*str = ft_substr(cmd_line, *position_dans_la_string, i);
	*position_dans_la_string += i + (delimiter == AND_OPERATOR || delimiter == OR_OPERATOR);
	if (cmd_line[i])
		*position_dans_la_string += 1;
	return (delimiter);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	char *new_line = NULL;
	int pos = 0;
	int delimiter = trouve_la_prochaine_portion_a_etudier(av[1], &new_line, &pos);
	printf("Portion de commande '%s'\nLe delimiter est: %i\n" , new_line, delimiter);
	printf("on est sur: '%c'\n", av[1][pos]);
	return (0);
}


str = "(echo 'cat ; (end | cat)) | cat &&' (cat | cat) > output"


char *str;

str = "(< input cat ; (end | cat)) | cat && (cat | cat) > output";

func (t_block **curr_block, char *str_to_analyse)
{
	int i = 0;
	next_param = get_next_param(&i)
	while (/* str_to_analyse[i] */is_valid_param(next_param))
	{
		if (parenthesis)
			// block = malloc(t_block);
			// block->cmd_line =  
			add_block(&curr_block, ft_find_closing_parenthesis(),last_sibling);
			sub_block = malloc(t_block);
			append_block_Back(sub_block, last_sub);
			func(sub_block, ft_find_closing_parenthesis);
			go_to_next_closing_parenthesis();
			next_param = get_next_param(&i)
			continue ;
		record_next_param(next_param);
		if (DELIM)
			append_block_back(malloc(t_block), last_sibling);
			go_after_delim();
		next_param = get_next_param(&i)
	}
	if (str_to_analyse[i])
		error();
	return (0);
}


t_block *head = NULL;

char *line = change_dollar_var(rl_line_buffer);
func(&head, line);