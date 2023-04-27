/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:56:26 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 00:39:57 by malfwa           ###   ########.fr       */
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

void		export(t_minishell *ms_params, char **tab, bool temp, t_fd fd);
void		unset(t_env_var **head, char **tab);
void		env(t_env_var *lst, t_fd fd);

// manage_env_var.c

t_env_var	*find_env_var(t_env_var *envp, char *str);
char		**build_envp(t_env_var	*envp);
char		*get_env_var_value(char *str);
char		*get_env_var_name(char *str);
void		update_env_var(t_env_var **head);

// export_utils.c

bool    	is_in_order(t_env_var *lst);
void	    swap_env_node(t_env_var **lst, t_env_var *a, t_env_var *b);
t_env_var	*sort_env(t_env_var *lst);
t_env_var	*cpy_t_env_var(t_env_var *lst);
bool	    add_update_env_var(char *name, t_minishell *ms_prms, \
bool temp, char *s);


#endif /* ENV_FUNCTION_C */