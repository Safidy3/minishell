/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 15:41:41 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_dir(DIR *dir, char *path)
{
	ft_putstr_fd(path, 2);
	if(dir != NULL)
		closedir(dir);
}

char	**get_new_command(char **command, t_all *all)
{
	char	**new_command;
	int		count;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	while (command[++i])
		if (command[i][0] != '>' && command[i][0] != '<')
			count++;
	new_command = (char **)calloc(sizeof(char *), count + 1);
	if (!new_command)
		exec_error(NULL, all, "malloc error\n");
	i = -1;
	while (command[++i])
		if (command[i][0] != '>' && command[i][0] != '<')
			new_command[++j] = command[i];
	return (new_command);
}

int exec_commands(t_all *all)
{
	t_list *command_list;
	pid_t pids[MAX_COMMANDS];
	int exit_stats[MAX_COMMANDS];
	int cmd_type[MAX_COMMANDS];
	int prev_fd[2];
	int current_fd[2];
	int command_count;

	t_redirect **redirections;
	char **command;
	char *bin_path;

	command_count = 0;
	prev_fd[0] = -1;
	prev_fd[1] = -1;


	int i = -1;
	while (++i < MAX_COMMANDS)
		cmd_type[i] = 0;

	command_list = all->command_list;
	command_count = ft_lstsize(all->command_list);

	command_count = 0;
	command_list = all->command_list;
	while (command_list)
	{
		if (command_list->next && pipe(current_fd) == -1)
			exec_error(NULL, all, "pipe creation failed\n");

		command = get_new_command((char **)command_list->content, all);
		if (!command)
			exec_error(NULL, all, "get_new_command failed\n");
		else
		{
			DIR *dir;
			if (ft_strchr(command[0], '/'))
			{
				dir = opendir(command[0]);
				if (dir || errno == EACCES)
				{
					close_dir(dir, command[0]);
					all->exit_status = 126;
					return (0);
					// error(": Is a directory\n", shell, envp, 126);
				}
				if (access(command[0], F_OK | X_OK) == -1)
				{
					if (errno == ENOENT)
					{
						close_dir(dir, command[0]);
						ft_putendl_fd(": No such file or directory", 2);
						all->exit_status = 127;
						return (0);
						// error(": No such file or directory\n", shell, envp, 127);
					}
					else if (errno == EACCES)
					{
						close_dir(dir, command[0]);
						ft_putendl_fd(": Permission denied", 2);
						all->exit_status = 126;
						return (0);
						// error(": Permission denied\n", shell, envp, 126);
					}
					else if (errno == ENOTDIR)
					{
						close_dir(dir, command[0]);
						ft_putendl_fd(": Not a directory", 2);
						all->exit_status = 126;
						return (0);
						// error(": Not a directory\n", shell, envp, 126);
					}
				}
			}
		}

		redirections = get_all_redirections((char **)command_list->content, all);
		int redir_val = manage_redirections(redirections, all);
		if (redir_val == 1)
			return (-1);
		else if (redir_val == -1)
		{
			free(command);
			exit_stats[command_count] = 1;
			cmd_type[command_count] = 1;
			command_list = command_list->next;
			command_count++;
			continue ;
		}

		bin_path = get_bin_path(command[0], all);
		if (!bin_path && !is_builtins(command[0]))
		{
			all->exit_status = command_not_found(command_list, command);
			return (1);
		}
		if (!ft_strcmp(command[0], "exit") || !ft_strcmp(command[0], "env") ||!ft_strcmp(command[0], "export") || !ft_strcmp(command[0], "unset"))
		{
			if (bin_path)
				free(bin_path);
			free(command);
			cmd_type[command_count] = 1;
			exit_stats[command_count] = exec_builtins(command_list, prev_fd, current_fd, all);
		}
		else
		{
			pids[command_count] = fork();
			if (pids[command_count] == 0)
			{
				if (prev_fd[0] != -1)
					dup_in(prev_fd, all, bin_path, 0);
				if (command_list->next)
					dup_out(current_fd, all, bin_path, 1);
				if (bin_path)
					execve(bin_path, command, all->env_arr);
				else if (is_builtins(command[0]))
				{
					exit_stats[command_count] = builtin_execution(command, all);
					free_all_struct(all);
					free(command);
					exit(exit_stats[command_count]);
				}
				exec_error(bin_path, all, "execve failed\n");
			}
			else if (pids[command_count] > 0)
			{
				if (prev_fd[0] != -1)
					close(prev_fd[0]);
				if (command_list->next)
				{
					close(current_fd[1]);
					prev_fd[0] = current_fd[0];
				}
				dup2(all->fd_og[0], STDIN_FILENO);
				dup2(all->fd_og[1], STDOUT_FILENO);
				ft_free(bin_path);
				free(command);
			}
			else
				exec_error(NULL, all, "fork failed\n");
		}
		command_list = command_list->next;
		command_count++;
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);

	i = -1;
	while (++i < command_count)
	{
		int status = 0;
		if (cmd_type[i] == 1)
			all->exit_status = exit_stats[i];
		else if (cmd_type[i] == 0)
		{
			waitpid(pids[i], &status, 0);
			all->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}

