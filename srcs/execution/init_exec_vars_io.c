/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_vars_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hateisse <hateisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:30:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/05/11 14:24:19 by hateisse         ###   ########.fr       */
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

static bool	check_init_exec_io_error(t_block *block, int ret)
{
	if (ret == -1)
		exit_ms(2, "exec init");
	else if (ret == -2)
	{
		block->cmd.exit_value = 1 << 8;
		g_ms_params.last_exit_code = block->cmd.exit_value;
		perror("minishell");
		set_env_exit_var(extract_exit_code(block->cmd.exit_value));
		errno = 0;
		return (false);
	}
	return (true);
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
	return (check_init_exec_io_error(block, ret));
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
