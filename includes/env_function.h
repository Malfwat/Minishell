/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_function.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 00:56:26 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/28 01:15:15 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_FUNCTION_H
# define ENV_FUNCTION_H

# include <struct_ms.h>

// init_env.c

t_env	*new_env_var(char *name, char *value, bool temp);
t_env	*get_last_env(t_env *tmp);
t_env	*get_env(char **env);
bool	add_env_var(t_env **head, char *name, char *value, bool temp);

// export_unset.c

void	export(t_minishell *ms_params, char **tab, bool temp, t_fd fd);
void	unset(t_env **head, char **tab);
void	env(t_env *lst, t_fd fd);

// manage_env_var.c

t_env	*find_env_var(t_env *envp, char *str);
char	**build_envp(t_env *envp);
char	*get_env_value(char *str);
char	*get_env_name(char *str);
void	update_env_var(t_env **head);

// export_utils.c

bool	is_in_order(t_env *lst);
void	swap_env_node(t_env **lst, t_env *a, t_env *b);
t_env	*sort_env(t_env *lst);
t_env	*cpy_t_env(t_env *lst);
bool	add_update_env_var(char *name, t_minishell *ms_prms, bool temp,
			char *s);

#endif /* ENV_FUNCTION_C */