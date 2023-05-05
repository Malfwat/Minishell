/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_vars_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malfwa <malfwa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:30:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/05 06:20:38 by malfwa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_exec.h>
#include <ms_env_function.h>
#include <stdio.h>
#include <ms_parsing.h>

char	**build_path(void)
{
	char	**path;
	t_env	*path_env_var;

	path = NULL;
	path_env_var = find_env_var(g_ms_params.envp, "PATH");
	if (path_env_var)
		path = ft_split(path_env_var->var_value, ':');
	if (errno)
		exit_ms(2, "exec_build");
	return (path);
}

char	**build_argv(t_args **head, char **path, t_env *envp)
{
	char	**tab;
	int		i;
	t_args	*tmp;

	i = 0;
	if (errno)
		return (NULL);
	rebuild_args(head, envp);
	update_t_args(head);
	check_for_color(head);
	get_cmd_path(path, &(*head)->final_arg, &(*head)->cmd_w_path);
	tab = ft_calloc(t_arg_lst_len(*head) + 1, sizeof(char *));
	if (!tab)
		return (0);
	i = 0;
	tmp = *head;
	while (tmp)
	{
		tab[i++] = tmp->final_arg;
		tmp = tmp->next;
	}
	return (tab);
}

char	**build_envp(t_env	*envp)
{
	char	**tab;
	int		len;
	t_env	*tmp;

	tmp = envp;
	len = 0;
	while (envp)
	{
		if (envp->env_scope == PUBLIC_VAR)
			len++;
		envp = envp->next;
	}
	tab = ft_calloc((len + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	len = 0;
	while (tmp)
	{
		if (tmp->env_scope == PUBLIC_VAR)
		{
			tab[len] = ft_strsjoin(3, tmp->var_name, "=", tmp->var_value);
			if (!tab[len++])
				return (ft_strsfree(tab), NULL);
		}
		tmp = tmp->next;
	}
	return (tab);
}

bool	init_exec_io(t_block *block)
{
	t_redirect	*tmp;
	int			ret;

	tmp = block->io_redirect;
	ret = 0;
	while (tmp && !errno)
	{
		if (tmp->mode == INPUT_MODE)
			ret = input_manager(tmp, &block->io_tab[0], \
			block, g_ms_params.envp);
		else if (tmp->mode == OUTPUT_MODE)
			ret = output_manager(tmp, &block->io_tab[1], g_ms_params.envp);
		tmp = tmp->next;
	}
	if (ret == -1)
		exit_ms(2, "exec init");
	else if (ret == -2)
	{
		block->cmd.exit_value = 1 << 8; 
		g_ms_params.last_exit_code = block->cmd.exit_value;
		perror("minishell1");
		set_env_exit_var(extract_exit_code(block->cmd.exit_value));
		errno = 0;
		return (false);
	}
	return (true);
}

t_exec_vars	init_exec_vars(t_block *block)
{
	t_exec_vars	exec_vars;

	exec_vars.cd_implicit = NULL;
	exec_vars.path = build_path();
	exec_vars.argv = build_argv \
	(&block->cmd.args, exec_vars.path, g_ms_params.envp);
	exec_vars.envp = build_envp(g_ms_params.envp);
	if (errno)
	{
		free_exec_vars(exec_vars);
		exit_ms(2, "exec_build");
	}
	return (exec_vars);
}
