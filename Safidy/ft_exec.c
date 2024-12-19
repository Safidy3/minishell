/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/18 17:09:47 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_dir(DIR *dir, char *path)
{
	ft_putstr_fd("bash: ", 2);
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
	if (count == 0)
		return (NULL);
	new_command = (char **)ft_calloc(sizeof(char *), count + 1);
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

/*********************** BUILTINS REDIRECTION ************************* */

int	builtins_output_redirection(t_redirect *redirect, t_all *all)
{
	int	fd;

	fd = 0;
	if (redirect->type == TRUNCATE)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(redirect->filename);
	if (fd != -1 && dup2(fd, STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int	builtin_redirections(t_all *all)
{
	int	fd;
	int	i;

	if (!all->redir)
		return (0);
	i = -1;
	while (all->redir[++i])
	{
		fd = -1;
		if (all->redir[i]->type == TRUNCATE || all->redir[i]->type == APPEND)
			fd = builtins_output_redirection(all->redir[i], all);
		else if (all->redir[i]->type == INPUT)
		{
			fd = open(all->redir[i]->filename, O_RDONLY);
			if (fd == -1)
				perror(all->redir[i]->filename);
		}
		if (fd == -1)
		{
			close(fd);
			all->exit_status = 1;
			break ;
		}
	}
	return (free_all_redir(all->redir), fd);
}

/********************************************************************* */

/*
to do :
fonction CALLOC ESORINA

unset HOME && cd ~

cat << "$US"E"R"

export var9="echo 'Hello world'"
$var9

export var10='echo "Hello world"'
$var10

HEREDOC ENV

*---> leaks
unset PWD OLDPWD
cd /
/bin/ls
pwd
echo $OLDPWD $PWD

*/

void	get_all_exit_stat(t_all *all, int command_count)
{
	int	i;
	int	status;

	i = -1;
	while (++i < command_count)
	{
		status = 0;
		waitpid(all->pids[i], &status, 0);
		if (WIFSIGNALED(status) || g_flag)
		{
			if (WTERMSIG(status) == SIGQUIT)
			{
				ft_putstr_fd("Quit (core dumped)\n", 2);
				all->exit_status = 128 + WTERMSIG(status);
			}
			else
			{
				ft_putstr_fd("\n", 1);
				all->exit_status = 128 + g_flag;
			}
			g_flag = 0;
		}
		else
			all->exit_status = WEXITSTATUS(status);
	}
}

void	exec_parent(t_all *all, t_list *command_list)
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
	free_all_redir(all->redir);
	if (all->command)
	{
		if (all->bin_path)
			free(all->bin_path);
		free(all->command);
	}
}

void	command_dir_error(t_all *all)
{
	int	exit_stat;

	exit_stat = all->exit_status;
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	free_all_redir(all->redir);
	ft_free(all->bin_path);
	ft_free(all->command);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(exit_stat);
}

void	get_command_bin(t_all *all, t_list *command_list)
{
	all->command = get_new_command(command_list, all);
	if (all->command)
	{
		if (!all->command[0][0])
			command_not_found(all);
		if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
			command_dir_error(all);
		all->bin_path = get_bin_path(all->command[0], all);
	}
	if (!all->bin_path && all->command && !is_builtins(all->command[0]))
		command_not_found(all);
	if (all->redir)
		manage_redirections(all->redir, all);
}

void	free_if_failed_exec(t_all *all)
{
	int	exit_stat;

	exit_stat = all->exit_status;
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(exit_stat);
}

void	exec_child(t_all *all, t_list *command_list)
{
	int	exit_stats;

	signal(SIGQUIT, SIG_DFL);
	if (all->in_pipe[0] != -1)
		dup_in(all->in_pipe, 0);
	if (command_list->next)
		dup_out(all->out_pipe, 1);
	get_command_bin(all, command_list);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	if (all->command)
	{
		if (is_builtins(all->command[0]))
		{
			exit_stats = builtin_execution(all->command, all);
			free(all->bin_path);
			free(all->command);
			free_all_struct(all);
			exit(exit_stats);
		}
		else if (all->bin_path)
			execve(all->bin_path, all->command, all->env_arr);
	}
	free_if_failed_exec(all);
}

void	exec_non_forked(t_all *all, t_list *command_list)
{
	all->command = get_new_command(command_list, all);
	if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
		return ;
	all->redir = get_all_redirections(command_list, all);
	if (builtin_redirections(all) != -1)
		all->exit_status = builtin_execution(all->command, all);
	restore_std(all);
	return (free(all->command));
}

void	init_t_all_var(t_all *all)
{
	int		i;

	i = -1;
	all->command = NULL;
	all->bin_path = NULL;
	all->in_pipe[0] = -1;
	all->in_pipe[1] = -1;
	while (++i < MAX_COMMANDS)
		all->pids[i] = 0;
}

int	manage_heredoc_sigdef(t_all *all)
{
	int		i;

	i = -1;
	if (all->redir)
		while (all->redir[++i])
			if (all->redir[i]->type == HEREDOC
				&& get_heredoc(all->redir[i]->filename,
					&all->redir[i]->fd, all))
				return (1);
	return (0);
}

void	exec_forked(t_all *all, t_list *command_list, int command_count)
{
	if (all->pids[command_count] == 0)
		exec_child(all, command_list);
	else if (all->pids[command_count] > 0)
		exec_parent(all, command_list);
	else
		exec_error(NULL, all, "fork failed\n");
}

int	exec_commands(t_all *all)
{
	t_list	*command_list;
	char	*cmd;
	int		command_count;

	command_count = 0;
	init_t_all_var(all);
	command_list = all->command_list;
	cmd = get_first_command(command_list);
	if (is_builtins(cmd) && command_list->next == NULL)
		return (exec_non_forked(all, command_list), 0);
	while (command_list)
	{
		if (command_list->next && pipe(all->out_pipe) == -1)
			exec_error(NULL, all, "pipe creation failed\n");
		all->redir = get_all_redirections(command_list, all);
		if (manage_heredoc_sigdef(all))
			return (1);
		all->pids[command_count] = fork();
		exec_forked(all, command_list, command_count);
		command_list = command_list->next;
		command_count++;
	}
	if (all->in_pipe[0] != -1)
		close(all->in_pipe[0]);
	return (get_all_exit_stat(all, command_count), 0);
}
