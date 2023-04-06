/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:56:26 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/06 05:20:17 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_FUNCTION_H
# define ENV_FUNCTION_H

# include <struct_ms.h>

// init_env_var.c

t_env_var	*new_env_var(char *name, char *value, bool temp);
t_env_var	*get_last_env_var(t_env_var *tmp);
void		free_env_lst(t_env_var **lst);
t_env_var	*get_env_var(char **env);
bool		add_env_var(t_env_var **head, char *name, char *value, bool temp);

// export_unset.c

void		export(t_env_var **lst, char *name, char *value, bool temp);
void		unset(t_env_var **head, t_env_var *to_pop);
void		env(t_env_var *lst);

// manage_env_var.c

t_env_var	*find_env_var(t_env_var *lst, char *str);
char		**t_env_var_to_array(t_env_var *lst);
char		*get_env_var_value(char *str);
char		*get_env_var_name(char *str);
void		update_env_var(t_env_var **head);

#endif /* ENV_FUNCTION_C */