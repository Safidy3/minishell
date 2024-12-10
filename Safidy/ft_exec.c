/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/10 13:25:26 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_dir(DIR *dir, char *path)
{
	ft_putstr_fd(path, 2);
	if (dir != NULL)
		closedir(dir);
}

char	**get_new_command(t_list *command_list, t_all *all)
{
	char	**new_command;
	int		count;
	char	**command;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	command = (char **)command_list->content;
	while (command[++i])
		if (command_list->type[i] == 0)
			count++;
	new_command = (char **)calloc(sizeof(char *), count + 1);
	if (!new_command)
		exec_error(NULL, all, "malloc error\n");
	i = -1;
	while (command[++i])
		if (command_list->type[i] == 0)
			new_command[++j] = command[i];
	return (new_command);
}

int	invalid_file(DIR *dir, char *command, t_all *all)
{
	if (errno == ENOENT)
	{
		close_dir(dir, command);
		ft_putendl_fd(": No such file or directory", 2);
		all->exit_status = 127;
		return (1);
	}
	else if (errno == EACCES)
	{
		close_dir(dir, command);
		ft_putendl_fd(": Permission denied", 2);
		all->exit_status = 126;
		return (1);
	}
	else if (errno == ENOTDIR)
	{
		close_dir(dir, command);
		ft_putendl_fd(": Not a directory", 2);
		all->exit_status = 126;
		return (1);
	}
	return (0);
}

int	is_dir(char *command, t_all *all)
{
	DIR	*dir;

	dir = opendir(command);
	if (dir || errno == EACCES)
	{
		close_dir(dir, command);
		ft_putendl_fd(": Is a directory", 2);
		all->exit_status = 126;
		return (1);
	}
	if (access(command, F_OK | X_OK) == -1)
		if (invalid_file(dir, command, all))
			return (1);
	return (0);
}

void	get_all_exit_stat(t_all *all, int command_count, t_cmd_utils *c_utils)
{
	int	i;
	int	status;

	i = -1;
	if (all->prev_fd[0] != -1)
		close(all->prev_fd[0]);
	while (++i < command_count)
	{
		status = 0;
		if (c_utils->cmd_type[i] == 1)
			all->exit_status = c_utils->exit_stats[i];
		else if (c_utils->cmd_type[i] == 0)
		{
			waitpid(c_utils->pids[i], &status, 0);
			all->exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec_child(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list, int command_count)
{
	char	**command;
	char	*bin_path;

	bin_path = c_utils->bin_path;
	command = c_utils->cmd;
	if (all->prev_fd[0] != -1)
		dup_in(all->prev_fd, 0);
	if (command_list->next)
		dup_out(all->current_fd, 1);
	if (bin_path)
		execve(bin_path, command, all->env_arr);
	else if (is_builtins(command[0]))
	{
		c_utils->exit_stats[command_count] = builtin_execution(command, all);
		free_all_struct(all);
		free(command);
		exit(c_utils->exit_stats[command_count]);
	}
}

void	exec_parent(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list)
{
	if (all->prev_fd[0] != -1)
		close(all->prev_fd[0]);
	if (command_list->next)
	{
		close(all->current_fd[1]);
		all->prev_fd[0] = all->current_fd[0];
	}
	dup2(all->fd_og[0], STDIN_FILENO);
	dup2(all->fd_og[1], STDOUT_FILENO);
	ft_free(c_utils->bin_path);
	free(c_utils->cmd);
}

int	exec_unfork_builtin(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list, int command_count)
{
	if (!ft_strcmp(c_utils->cmd[0], "exit")
		|| !ft_strcmp(c_utils->cmd[0], "env")
		|| !ft_strcmp(c_utils->cmd[0], "export")
		|| !ft_strcmp(c_utils->cmd[0], "unset")
		|| !ft_strcmp(c_utils->cmd[0], "pwd")
		|| !ft_strcmp(c_utils->cmd[0], "cd"))
	{
		if (c_utils->bin_path)
			free(c_utils->bin_path);
		free(c_utils->cmd);
		c_utils->cmd_type[command_count] = 1;
		c_utils->exit_stats[command_count] = exec_builtins(command_list,
				all->prev_fd, all->current_fd, all);
		return (1);
	}
	return (0);
}


void	command_execution(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list, int command_count)
{
	if (exec_unfork_builtin(all, c_utils, command_list, command_count))
		;
	else
	{
		c_utils->pids[command_count] = fork();
		if (c_utils->pids[command_count] == 0)
		{
			exec_child(all, c_utils, command_list, command_count);
			exec_error(c_utils->bin_path, all, "execve failed\n");
		}
		else if (c_utils->pids[command_count] > 0)
			exec_parent(all, c_utils, command_list);
		else
			exec_error(NULL, all, "fork failed\n");
	}
}

// int pre_exec(t_list *command_list, )
// {
// 	if (command_list->next && pipe(all->current_fd) == -1)
// 		exec_error(NULL, all, "pipe creation failed\n");
// 	c_utils->cmd = get_new_command(command_list, all);
// 	if (ft_strchr(c_utils->cmd[0], '/') && is_dir(c_utils->cmd[0], all))
// 		return (0);
// 	redir_val = manage_redirections(command_list, all);
// 	if (redir_val == 1)
// 		return (-1);
// 	else if (redir_val == -1)
// 	{
// 		free(c_utils->cmd);
// 		c_utils->exit_stats[command_count] = 1;
// 		c_utils->cmd_type[command_count] = 1;
// 		command_list = command_list->next;
// 		command_count++;
// 		continue ;
// 	}
// 	c_utils->bin_path = get_bin_path(c_utils->cmd[0], all);
// 	if (!c_utils->bin_path && !is_builtins(c_utils->cmd[0]))
// 	{
// 		all->exit_status = command_not_found(command_list, c_utils->cmd);
// 		return (1);
// 	}
// }

int	exec_commands(t_all *all)
{
	t_list		*command_list;
	t_cmd_utils	*c_utils;
	int			command_count;
	int			i;
	int	redir_val;

	c_utils = (t_cmd_utils *)malloc(sizeof(t_cmd_utils));
	i = -1;
	while (++i < MAX_COMMANDS)
		c_utils->cmd_type[i] = 0;
	command_list = all->command_list;
	command_count = 0;
	while (command_list)
	{
		if (command_list->next && pipe(all->current_fd) == -1)
			exec_error(NULL, all, "pipe creation failed\n");
		c_utils->cmd = get_new_command(command_list, all);
		if (ft_strchr(c_utils->cmd[0], '/') && is_dir(c_utils->cmd[0], all))
			return (0);
		redir_val = manage_redirections(command_list, all);
		if (redir_val == 1)
			return (-1);
		else if (redir_val == -1)
		{
			free(c_utils->cmd);
			c_utils->exit_stats[command_count] = 1;
			c_utils->cmd_type[command_count] = 1;
			command_list = command_list->next;
			command_count++;
			continue ;
		}
		c_utils->bin_path = get_bin_path(c_utils->cmd[0], all);
		if (!c_utils->bin_path && !is_builtins(c_utils->cmd[0]))
		{
			all->exit_status = command_not_found(command_list, c_utils->cmd);
			return (1);
		}
		command_execution(all, c_utils, command_list, command_count);
		command_list = command_list->next;
		command_count++;
	}
	get_all_exit_stat(all, command_count, c_utils);
	return (0);
}


// int	exec_commands(t_all *all)
// {
// 	t_list		*command_list;
// 	t_cmd_utils	*c_utils;
// 	int			command_count;
// 	int			i;
// 	int	redir_val;

// 	c_utils = (t_cmd_utils *)malloc(sizeof(t_cmd_utils));
// 	i = -1;
// 	while (++i < MAX_COMMANDS)
// 		c_utils->cmd_type[i] = 0;
// 	command_list = all->command_list;
// 	command_count = 0;
// 	while (command_list)
// 	{
// 		if (command_list->next && pipe(all->current_fd) == -1)
// 			exec_error(NULL, all, "pipe creation failed\n");
// 		c_utils->cmd = get_new_command(command_list, all);
// 		if (ft_strchr(c_utils->cmd[0], '/') && is_dir(c_utils->cmd[0], all))
// 			return (0);
// 		redir_val = manage_redirections(command_list, all);
// 		if (redir_val == 1)
// 			return (-1);
// 		else if (redir_val == -1)
// 		{
// 			free(c_utils->cmd);
// 			c_utils->exit_stats[command_count] = 1;
// 			c_utils->cmd_type[command_count] = 1;
// 			command_list = command_list->next;
// 			command_count++;
// 			continue ;
// 		}
// 		c_utils->bin_path = get_bin_path(c_utils->cmd[0], all);
// 		if (!c_utils->bin_path && !is_builtins(c_utils->cmd[0]))
// 		{
// 			all->exit_status = command_not_found(command_list, c_utils->cmd);
// 			return (1);
// 		}
// 		command_execution(all, c_utils, command_list, command_count);
// 		command_list = command_list->next;
// 		command_count++;
// 	}
// 	get_all_exit_stat(all, command_count, c_utils);
// 	return (0);
// }

