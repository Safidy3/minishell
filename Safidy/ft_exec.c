/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/13 14:42:09 by larakoto         ###   ########.fr       */
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
	if (all->in_pipe[0] != -1)
		close(all->in_pipe[0]);
	while (++i < command_count)
	{
		status = 0;
		if (c_utils->cmd_type[i] == 1)
			all->exit_status = c_utils->exit_stats[i];
		else if (c_utils->cmd_type[i] == 0)
		{
			waitpid(c_utils->pids[i], &status, 0);
			if (WIFSIGNALED(status))
				printf("Quit (core dumped)\n");
			all->exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec_child(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list, int command_count)
{
	char	**command;
	char	*bin_path;
	int		exit_stat;

	bin_path = c_utils->bin_path;
	command = c_utils->cmd;
	if (all->in_pipe[0] != -1)
		dup_in(all->in_pipe, 0);
	if (command_list->next)
		dup_out(all->out_pipe, 1);
	if (bin_path)
	{
		free(c_utils);
		execve(bin_path, command, all->env_arr);
	}
	else if (is_builtins(command[0]))
	{
		exit_stat = builtin_execution(command, all);
		c_utils->exit_stats[command_count] = exit_stat;
		free_all_struct(all);
		free(c_utils);
		free(command);
		exit(exit_stat);
	}
}

void	exec_parent(t_all *all, t_cmd_utils *c_utils,
			t_list *command_list)
{
	if (all->in_pipe[0] != -1)
		close(all->in_pipe[0]);
	if (command_list->next)
	{
		close(all->out_pipe[1]);
		all->in_pipe[0] = all->out_pipe[0];
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
				all->in_pipe, all->out_pipe, all);
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
			signal(SIGQUIT, SIG_DFL);
			exec_child(all, c_utils, command_list, command_count);
			exec_error(c_utils->bin_path, all, "execve failed\n");
		}
		else if (c_utils->pids[command_count] > 0)
			exec_parent(all, c_utils, command_list);
		else
			exec_error(NULL, all, "fork failed\n");
	}
}

char	*get_first_command(t_list *command_list)
{
	char	**command;
	int		i;

	command = (char **)command_list->content;
	i = -1;
	while (command[++i])
		if (command_list->type[i] == 0)
			return (command[i]);
	return (NULL);
}

void	restore_std(t_all *all)
{
	if (dup2(all->fd_og[0], STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdin failed\n");
	if (dup2(all->fd_og[1], STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdout failed\n");
}

int builtin_redirections(t_list *command_list, t_all *all)
{
	t_redirect	**redir;
	int         fd_out;
	int         i;

	fd_out = -1;
	redir = get_all_redirections(command_list, all);
	if (!redir)
		return (0);
	i = -1;
	while (redir[++i])
	{
		if (redir[i]->type == INPUT)
		{
			DIR	*dir;

			dir = opendir(redir[i]->filename);
			if (dir || errno == EACCES)
			{
				close_dir(dir, redir[i]->filename);
				ft_putendl_fd(": Is a directory", 2);
				all->exit_status = 126;
				return (1);
			}
			if (is_dir(redir[i]->filename, all))
				return (0);
		}
		if (redir[i]->type == TRUNCATE || redir[i]->type == APPEND)
		{
			if (redir[i]->type == TRUNCATE)
				fd_out = open(redir[i]->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (redir[i]->type == APPEND)
				fd_out = open(redir[i]->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out == -1)
			{
				perror("Error opening output file (truncate)");
				continue;
			}
			if (dup2(fd_out, STDOUT_FILENO) == -1)
			{
				perror("Error redirecting stdout");
				close(fd_out);
			}
			close(fd_out);
		}
		ft_free(redir[i]->filename);
		free(redir[i]);
	}
	return (free(redir), 1);
}


// mande : echo hi >./outfiles/outfile01 | echo bye >./outfiles/outfile02
// tsy mande : cat <missing | cat <"./test_files/infile"
// todo : fix pipe

/* TMP EXEC TALOA*/

int exec_commands(t_all *all)
{
	t_list *command_list;
	pid_t pids[MAX_COMMANDS];
	int exit_stats[MAX_COMMANDS];
	int cmd_type[MAX_COMMANDS];
	int in_pipe[2];
	int out_pipe[2];
	int command_count;
	t_redirect	**redir;

	char **command;
	char *bin_path;

	command_count = 0;
	in_pipe[0] = -1;
	in_pipe[1] = -1;

	int i = -1;
	while (++i < MAX_COMMANDS)
		cmd_type[i] = 0;
	command_count = 0;
	command_list = all->command_list;

	/* exec builtins fotsiny */
	
	char *cmd = get_first_command(command_list);
	if (is_builtins(cmd) && command_list->next == NULL)
	{
		command = get_new_command(command_list, all);
		if (ft_strchr(command[0], '/') && is_dir(command[0], all))
			return (0);
		if (builtin_redirections(command_list, all) != -1)
			all->exit_status = builtin_execution(command, all);
		restore_std(all);
		return (free(command), 0);
	}
	



	/* exec misy pipe sy non_builtin */
	while (command_list)
	{
		if (command_list->next && pipe(out_pipe) == -1)
			exec_error(NULL, all, "pipe creation failed\n");
		command = get_new_command(command_list, all);
		if (ft_strchr(command[0], '/') && is_dir(command[0], all))
			return (0);
		bin_path = get_bin_path(command[0], all);
		if (!bin_path && !is_builtins(command[0]))
		{
			all->exit_status = command_not_found(command_list, command);
			return (1);
		}
		redir = get_all_redirections(command_list, all);
		if (redir)
		{
			i = -1;
			while (redir[++i])
				if (redir[i]->type == HEREDOC)
					if (get_heredoc(redir[i]->filename, &redir[i]->fd, all))
						return (1);
		}

		pids[command_count] = fork();
		if (pids[command_count] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			if (in_pipe[0] != -1)
				dup_in(in_pipe, 0);
			if (command_list->next)
				dup_out(out_pipe, 1);
			if (redir)
			{
				int redir_val = manage_redirections(redir, all);
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
			}
			if (is_builtins(command[0]))
			{
				exit_stats[command_count] = builtin_execution(command, all);
				free_all_struct(all);
				free(command);
				exit(exit_stats[command_count]);
			}
			else if (bin_path)
				execve(bin_path, command, all->env_arr);
			///modification
			ft_free(bin_path);
			free_list(all->command_list);
			free_split(all->env_arr);
			ft_free_env_list(all->env_list);
			int exit_stat = all->exit_status;
			free(all);
			exit(exit_stat);
		}
		else if (pids[command_count] > 0)
		{
			if (in_pipe[0] != -1)
				close(in_pipe[0]);
			if (command_list->next)
			{
				close(out_pipe[1]);
				in_pipe[0] = out_pipe[0];
			}
			dup2(all->fd_og[0], STDIN_FILENO);
			dup2(all->fd_og[1], STDOUT_FILENO);
			ft_free(bin_path);
			free(command);
		}
		else
			exec_error(NULL, all, "fork failed\n");
		command_list = command_list->next;
		command_count++;
	}
	if (in_pipe[0] != -1)
		close(in_pipe[0]);

	i = -1;
	while (++i < command_count)
	{
		int status = 0;
		if (cmd_type[i] == 1)
			all->exit_status = exit_stats[i];
		else if (cmd_type[i] == 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				all->exit_status = 131;
			}
			else
				all->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}


// tsy mande : cat <missing | cat <"./test_files/infile"
// int exec_commands(t_all *all)
// {
// 	t_list *command_list;
// 	pid_t pids[MAX_COMMANDS];
// 	int exit_stats[MAX_COMMANDS];
// 	int cmd_type[MAX_COMMANDS];
// 	// int in_pipe[2];
// 	// int out_pipe[2];

// 	int i;
// 	int command_count;
// 	char **command;
// 	char *bin_path;

// 	all->in_pipe[0] = -1;
// 	all->in_pipe[1] = -1;
// 	all->out_pipe[0] = -1;
// 	all->out_pipe[1] = -1;

// 	i = -1;
// 	while (++i < MAX_COMMANDS)
// 		cmd_type[i] = 0;

// 	command_count = 0;
// 	command_list = all->command_list;
// 	while (command_list)
// 	{
// 		command = get_new_command(command_list, all);
// 		bin_path = get_bin_path(command[0], all);
// 		if (command_list->next && pipe(all->out_pipe) == -1)
// 		{
//  			perror("Pipe creation failed");
// 			exec_error(NULL, all, "pipe creation failed\n");
// 		}
// 		pids[command_count] = fork();
// 		if (pids[command_count] == 0)
// 		{
// 			signal(SIGQUIT, SIG_DFL);
// 			// Use in_pipe as input if it exists
//             if (all->in_pipe[0] != -1)
// 			{
//                 dup2(all->in_pipe[0], STDIN_FILENO);
//                 close(all->in_pipe[0]);
//                 close(all->in_pipe[1]);
//             }
//             // Use out_pipe for output if there's a next command
//             if (command_list->next)
// 			{
//                 close(all->out_pipe[0]);
//                 dup2(all->out_pipe[1], STDOUT_FILENO);
//                 close(all->out_pipe[1]);
//             }
// 			// manage_redirections(command_list, all);
// 			if (is_builtins(command[0]))
// 			{
// 				exit_stats[command_count] = builtin_execution(command, all);
// 				free_all_struct(all);
// 				free(command);
// 				exit(exit_stats[command_count]);
// 			}
// 			else if (bin_path)
// 				execve(bin_path, command, all->env_arr);
// 		}
// 		else if (pids[command_count] > 0)
// 		{
// 			// Close input pipe if it was used
// 			if (all->in_pipe[0] != -1)
// 			{
// 				close(all->in_pipe[0]);
// 				close(all->in_pipe[1]);
// 			}
// 			// Move out_pipe to in_pipe if there's a next command
// 			if (command_list->next)
// 			{
// 				// Close write end of out_pipe
// 				close(all->out_pipe[1]);
// 				// Store out_pipe as in_pipe for next iteration
// 				all->in_pipe[0] = all->out_pipe[0];
// 				all->in_pipe[1] = all->out_pipe[1];
// 				// Reset out_pipe
// 				all->out_pipe[0] = -1;
// 				all->out_pipe[1] = -1;
// 			}
// 			dup2(all->fd_og[0], STDIN_FILENO);
// 			dup2(all->fd_og[1], STDOUT_FILENO);
// 			ft_free(bin_path);
// 			free(command);
// 		}
// 		else
// 			exec_error(NULL, all, "fork failed\n");
// 		command_list = command_list->next;
// 		command_count++;
// 	}
// 	if (all->in_pipe[0] != -1)
// 		close(all->in_pipe[0]);
// 	i = -1;
// 	while (++i < command_count)
// 	{
// 		int status = 0;
// 		if (cmd_type[i] == 1)
// 			all->exit_status = exit_stats[i];
// 		else if (cmd_type[i] == 0)
// 		{
// 			waitpid(pids[i], &status, 0);
// 			all->exit_status = WEXITSTATUS(status);
// 		}
// 	}
// 	return (0);
// }
