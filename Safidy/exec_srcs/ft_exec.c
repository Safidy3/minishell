/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:25:18 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/19 14:12:08 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
