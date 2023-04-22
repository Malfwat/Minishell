/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_ins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:09:00 by hateisse          #+#    #+#             */
/*   Updated: 2023/04/22 16:43:56 by amouflet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <struct_ms.h>
#include <env_function.h>
#include <exec_ms.h>
#include <ms_define.h>

bool	is_builtin(char *str)
{
	if (!ft_strcmp(str, "env"))
		return (true);
	if (!ft_strcmp(str, "pwd"))
		return (true);
	if (!ft_strcmp(str, "unset"))
		return (true);
	if (!ft_strcmp(str, "export"))
		return (true);
	if (!ft_strcmp(str, "cd"))
		return (true);
	if (!ft_strcmp(str, "echo"))
		return (true);
	return (false);
}

void	launch_builtins(t_minishell *ms_params, t_exec_vars vars)
{
	char	*str;

	str = vars.argv[0];
	if (!ft_strcmp(str, "env"))
		env(ms_params->envp);
	else if (!ft_strcmp(str, "pwd"))
		pwd();
	else if (!ft_strcmp(str, "unset"))
		unset(&ms_params->envp, &vars.argv[1]);
	else if (!ft_strcmp(str, "export"))
		export(ms_params, &ms_params->envp, &vars.argv[1], 0);
	else if (!ft_strcmp(str, "cd"))
		cd(ms_params, ms_params->envp, &vars.argv[1]);
	else if (!ft_strcmp(str, "echo"))
		ms_echo(&vars.argv[1]);
}

void	exec_builtin(t_block *block, t_minishell *ms_params, t_exec_vars vars)
{
	launch_builtins(ms_params, vars);
	block->cmd.exit_value = ms_params->last_exit_code;
	free(find_env_var(ms_params->envp, "?")->var_value);
	find_env_var(ms_params->envp, "?")->var_value = ft_itoa(block->cmd.exit_value);
	if (block->io_tab[0] >= 0)
		close(block->io_tab[0]);
	if (block->io_tab[1] >= 0)
		close(block->io_tab[1]);
	free_exec_vars(vars);
	if (errno)
		exit_ms(*ms_params, 2, "builtins");
	block->cmd.exit_value = 0;
	// ms_params->last_exit_code = 0;
}
