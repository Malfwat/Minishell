/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:56:26 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/04 16:57:46 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_FUNCTION_C
# define ENV_FUNCTION_C

#include <struct_ms.h>


// init_env_var.c

t_env_var	*new_env_var(char *str);
t_env_var	*get_last_env_var(t_env_var *tmp);
void		free_env_lst(t_env_var **lst);
t_env_var	*get_env_var(char **env);
bool		add_env_var(t_env_var **head, char *str);

// export_unset.c

int			export(t_env_var **lst, char *str);
int			unset(t_env_var **head, char *name);
void		env(t_env_var *lst);

// manage_env_var.c

t_env_var	*find_env_var(t_env_var *lst, char *str);
char		**t_env_var_to_array(t_env_var *lst);
char		*get_env_var_value(char *str);
char		*get_env_var_name(char *str);

#endif /* ENV_FUNCTION_C */