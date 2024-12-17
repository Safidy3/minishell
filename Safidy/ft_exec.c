/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/16 17:35:26 by safandri         ###   ########.fr       */
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
	if (count == 0)
		return (NULL);
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
			// if (WIFSIGNALED(status))
			// 	printf("Quit (core dumped)\n");
			all->exit_status = WEXITSTATUS(status);
		}
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

/*********************** BUILTINS REDIRECTION ************************* */

int	builtins_output_redirection(t_redirect *redirect,  t_all *all)
{
	int	fd;

	fd = 0;
	if (redirect->type == TRUNCATE)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(redirect->filename);
		// fd_error(redirect->filename, redir_head, all);
	if (fd != -1 && dup2(fd, STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int builtin_redirections(t_list *command_list, t_all *all)
{
	int			fd;
	int         i;

	all->redir = get_all_redirections(command_list, all);
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
		if (fd != -1)
			close(fd);
		if (fd == -1)
		{
			all->exit_status = 1;
			break;
		}
	}
	return (free_all_redir(all->redir), fd);
}

/********************************************************************* */

/*
to do :
fonction CALLOC ESORINA

exit tester_2

unset HOME && cd ~
cat << $a

cat << g
<$HOME

*/

int exec_commands(t_all *all)
{
	t_list *command_list;
	pid_t pids[MAX_COMMANDS];
	int exit_stats[MAX_COMMANDS];
	int cmd_type[MAX_COMMANDS];
	int in_pipe[2];
	int out_pipe[2];
	int command_count;

	all->command = NULL;
	all->bin_path = NULL;

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
		all->command = get_new_command(command_list, all);
		if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
			return (0);
		if (builtin_redirections(command_list, all) != -1)
			all->exit_status = builtin_execution(all->command, all);
		restore_std(all);
		return (free(all->command), 0);
	}

	/* exec misy pipe sy non_builtin */
	while (command_list)
	{
		if (command_list->next && pipe(out_pipe) == -1)
			exec_error(NULL, all, "pipe creation failed\n");

		all->redir = get_all_redirections(command_list, all);
		i = -1;
		if (all->redir)
			while (all->redir[++i])
				if (all->redir[i]->type == HEREDOC && get_heredoc(all->redir[i]->filename, &all->redir[i]->fd, all))
					return (1);

		pids[command_count] = fork();
		if (pids[command_count] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			if (in_pipe[0] != -1)
				dup_in(in_pipe, 0);
			if (command_list->next)
				dup_out(out_pipe, 1);

			all->command = get_new_command(command_list, all);
			if (all->command)
			{
				if (!all->command[0][0])
					command_not_found(all);
				if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
				{
					int exit_stat = all->exit_status;

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
				all->bin_path = get_bin_path(all->command[0], all);
			}
			if (!all->bin_path && all->command && !is_builtins(all->command[0]))
				command_not_found(all);
			if (all->redir)
				manage_redirections(all->redir, all);

			close(all->fd_og[0]);
			close(all->fd_og[1]);
			if (all->command)
			{
				if (is_builtins(all->command[0]))
				{
					exit_stats[command_count] = builtin_execution(all->command, all);
					free(all->bin_path);
					free(all->command);
					free_all_struct(all);
					exit(exit_stats[command_count]);
				}
				else if (all->bin_path)
					execve(all->bin_path, all->command, all->env_arr);
			}
			///modification
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
			free_all_redir(all->redir);
			if (all->command)
			{
				if (all->bin_path)
					free(all->bin_path);
				free(all->command);
			}
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
			if (WIFSIGNALED(status) || flag)
			{
				if (WTERMSIG(status) == SIGQUIT)
				{
					printf("Quit (core dumped)\n");
					all->exit_status = 128 + WTERMSIG(status);
				}
				else
				{
					printf("\n");
					all->exit_status = 128 + flag;
				}
				flag = 0;
			}
			else
				all->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}

/*

>: ./minishell
>: exit 42 5
exit: too many arguments
>: exit
exit
>: ./minishell
>: exit
exit
>: 
>: exit
exit
==9539== 
==9539== FILE DESCRIPTORS: 3 open (3 std) at exit.
==9539== 
==9539== HEAP SUMMARY:
==9539==     in use at exit: 228,836 bytes in 247 blocks
==9539==   total heap usage: 1,046 allocs, 799 frees, 300,514 bytes allocated
==9539== 
==9539== 1 bytes in 1 blocks are definitely lost in loss record 1 of 79
==9539==    at 0x484280F: malloc (vg_replace_malloc.c:442)
==9539==    by 0x407C54: ft_strdup (in /home/nobara/Documents/minishell/Safidy/minishell)
==9539==    by 0x401BE6: replace_env_vars (minishell.c:215)
==9539==    by 0x404644: main (main.c:115)

*/