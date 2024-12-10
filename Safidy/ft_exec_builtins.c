/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/10 10:01:55 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(char *command)
{
	if (!ft_strcmp(command, "export")
		|| !ft_strcmp(command, "unset")
		|| !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "echo")
		|| !ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "pwd")
		|| !ft_strcmp(command, "exit"))
		return (1);
	return (0);
}

void	restore_og_std(int std_backup[2], t_all *all)
{
	if (dup2(std_backup[0], STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdin failed\n");
	if (dup2(std_backup[1], STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdout failed\n");
	close(std_backup[0]);
	close(std_backup[1]);
}

int	builtin_execution(char **command, t_all *all)
{
	int	exit_status;

	exit_status = 0;
	if (!ft_strncmp(command[0], "export", ft_strlen("export")))
	{
		if (array_len(command) == 1)
			exit_status = ft_print_export(all->env_list);
		else
			exit_status = ft_export(&all->env_list, command);
	}
	else if (!ft_strncmp(command[0], "unset", ft_strlen("unset")))
		exit_status = ft_unset(&all->env_list, command);
	else if (!ft_strncmp(command[0], "env", ft_strlen("env")))
		exit_status = ft_print_env(all->env_list);
	else if (!ft_strncmp(command[0], "echo", ft_strlen("echo")))
		exit_status = ft_echo(command);
	else if (!ft_strncmp(command[0], "cd", ft_strlen("cd")))
		exit_status = ft_cd(command, all);
	else if (!ft_strncmp(command[0], "pwd", ft_strlen("pwd")))
		exit_status = ft_pwd();
	else if (!ft_strncmp(command[0], "exit", ft_strlen("exit")))
		ft_exit(all, command);
	return (exit_status);
}

int	exec_builtins(t_list *command_list, int prev_fd[2], int current_fd[2],
		t_all *all)
{
	char	**command;
	int		exit_stat;
	int		std_backup[2];

	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	if (std_backup[0] == -1 || std_backup[1] == -1)
		exec_error(NULL, all, "dup backup failed\n");
	if (prev_fd[0] != -1)
		dup_in(prev_fd, 0);
	if (command_list->next && current_fd[1] != -1)
		dup_out(current_fd, 0);
	command = (char **)command_list->content;
	exit_stat = builtin_execution(command, all);
	restore_og_std(std_backup, all);
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command_list->next && current_fd[1] != -1)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
	return (exit_stat);
}
