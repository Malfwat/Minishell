/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_vars_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:30:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/27 23:57:28 by hateisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exec_ms.h>
#include <env_function.h>
#include <stdio.h>
#include <parsing_ms.h>

char	**build_path(t_minishell ms_params)
{
	char	**path;

	path = ft_split(find_env_var(ms_params.envp, "PATH")->var_value, ':');
	if (errno)
		exit_ms(ms_params, 2, "exec_build");
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
		envp = envp->next;
		len++;
	}
	tab = ft_calloc((len + 1), sizeof(char *));
	if (!tab)
		return (NULL);
	len = 0;
	while (tmp)
	{
		tab[len] = ft_strsjoin(3, tmp->var_name, "=", tmp->var_value);
		if (!tab[len++])
			return (ft_strsfree(tab), NULL);
		tmp = tmp->next;
	}
	return (tab);
}

bool	init_exec_io(t_block *block, t_minishell *ms_params)
{
	t_redirect	*tmp;
	int			ret;

	tmp = block->io_redirect;
	ret = 0;
	while (tmp && !errno)
	{
		if (tmp->mode == INPUT_MODE)
			ret = input_manager(tmp, &block->io_tab[0], block, ms_params->envp);
		else if (tmp->mode == OUTPUT_MODE)
			ret = output_manager(tmp, &block->io_tab[1], ms_params->envp);
		tmp = tmp->next;
	}
	if (ret == -1)
		exit_ms(*ms_params, 2, "exec init");
	else if (ret == -2)
	{
		block->cmd.exit_value = SET_EXIT_CODE(1);
		ms_params->last_exit_code = block->cmd.exit_value;
		perror("minishell1");
		errno = 0;
		return (false);
	}
	return (true);
}

t_exec_vars	init_exec_vars(t_minishell ms_params, t_block *block)
{
	t_exec_vars	exec_vars;

	exec_vars.cd_implicit = NULL;
	exec_vars.path = build_path(ms_params);
	exec_vars.argv = build_argv \
	(&block->cmd.args, exec_vars.path, ms_params.envp);
	exec_vars.envp = build_envp(ms_params.envp);
	if (errno)
	{
		free_exec_vars(exec_vars);
		exit_ms(ms_params, 2, "exec_build");
	}
	return (exec_vars);
}
