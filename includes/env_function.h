/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:56:26 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 02:33:06 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_FUNCTION_H
# define ENV_FUNCTION_H

# include <struct_ms.h>

// init_env_var.c

t_env_var	*new_env_var(char *name, char *value, bool temp);
t_env_var	*get_last_env_var(t_env_var *tmp);
t_env_var	*get_env_var(char **env);
bool		add_env_var(t_env_var **head, char *name, char *value, bool temp);

// export_unset.c

void		export(t_env_var **lst, char **tab, bool temp);
void		unset(t_env_var **head, char **tab);
void		env(t_env_var *lst);

// manage_env_var.c

t_env_var	*find_env_var(t_env_var *envp, char *str);
char		**build_envp(t_env_var	*envp);
char		*get_env_var_value(char *str);
char		*get_env_var_name(char *str);
void		update_env_var(t_env_var **head);

#endif /* ENV_FUNCTION_C */