/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_vars_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:30:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/22 17:25:34 by hateisse         ###   ########.fr       */
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

char	**build_argv(t_args **head)
{
	char	**tab;
	int		len;
	t_args	*tmp;

	len = 0;
	if (errno)
		return (NULL);
	update_t_args(head);
	tmp = *head;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	tab = ft_calloc(len + 1, sizeof(char *));
	if (!tab)
		return (0);
	len = 0;
	tmp = *head;
	while (tmp)
	{
		tab[len++] = tmp->final_arg;
		tmp = tmp->next;
	}
	return (tab);
}

char	**build_envp(t_env_var	*envp)
{
	char		**tab;
	int			len;
	t_env_var	*tmp;

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
		block->cmd.exit_value = 1;
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
	char		*tmp;

	if (!rebuild_args(&block->cmd.args, ms_params.envp))
		exit_ms(ms_params, 2, "exec_build");
	exec_vars.path = build_path(ms_params);
	// if (!block->cmd.args)
	// {
	// 	block->cmd.args = new_cmd_arg(NULL);
	// 	block->cmd.args->final_arg = ft_strdup("");
	// }
	get_cmd_path(exec_vars.path, &block->cmd.args->final_arg, &tmp);
	block->cmd.args->final_arg = tmp;
	exec_vars.argv = build_argv(&block->cmd.args);
	exec_vars.envp = build_envp(ms_params.envp);
	if (errno)
	{
		free_exec_vars(exec_vars);
		exit_ms(ms_params, 2, "exec_build");
	}
	return (exec_vars);
}
