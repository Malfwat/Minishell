/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_vars_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouflet <amouflet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 05:30:33 by malfwa            #+#    #+#             */
/*   Updated: 2023/04/26 19:21:07 by amouflet         ###   ########.fr       */
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

bool	is_colorable(char *str)
{
	if (!ft_strcmp(str, "ls"))
		return (true);
	if (!ft_strcmp(str, "grep"))
		return (true);
	if (!ft_strcmp(str, "egrep"))
		return (true);
	if (!ft_strcmp(str, "fgrep"))
		return (true);
	if (!ft_strcmp(str, "diff"))
		return (true);
	if (!ft_strcmp(str, "ip"))
		return (true);
	return (false);
}

void	check_for_color(t_args **head)
{
	t_args	*tmp;

	if (is_colorable((*head)->final_arg))
	{
		if (!(*head)->next || ft_strncmp((*head)->next->final_arg, "--color=", 8))
		{
			tmp = new_cmd_arg(NULL);
			tmp->final_arg = ft_strdup("--color=auto");
			if (!tmp->final_arg)
				return ;
			tmp->prev = *head;
			if ((*head)->next)
				(*head)->next->prev = tmp;
			tmp->next = (*head)->next;
			(*head)->next = tmp;
		}
	}
}

char	**build_argv(t_args **head, char **path, t_env_var *envp)
{
	char	**tab;
	int		len;
	char	*str;
	t_args	*tmp;

	len = 0;
	if (errno)
		return (NULL);
	update_t_args(head);
	rebuild_args(head, envp);
	check_for_color(head);
	get_cmd_path(path, &(*head)->final_arg, &str);
	(*head)->cmd_w_path = str;
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
	// char		*tmp;

	// if (!rebuild_args(&block->cmd.args, ms_params.envp))
		// exit_ms(ms_params, 2, "exec_build");
	exec_vars.path = build_path(ms_params);
	// if (!block->cmd.args)
	// {
	// 	block->cmd.args = new_cmd_arg(NULL);
	// 	block->cmd.args->final_arg = ft_strdup("");
	// }

	exec_vars.argv = build_argv(&block->cmd.args, exec_vars.path, ms_params.envp);
	exec_vars.envp = build_envp(ms_params.envp);
	if (errno)
	{
		free_exec_vars(exec_vars);
		exit_ms(ms_params, 2, "exec_build");
	}
	return (exec_vars);
}
