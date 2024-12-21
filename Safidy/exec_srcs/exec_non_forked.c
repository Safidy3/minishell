/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_forked.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:11:26 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 10:58:29 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_redir(t_redirect **redirect)
{
	int	i;

	if (!redirect)
		return (0);
	i = -1;
	while (redirect[++i])
		if (redirect[i]->type == TRUNCATE || redirect[i]->type == APPEND
			|| redirect[i]->type == INPUT)
			return (1);
	return (free_all_redir(redirect), 0);
}

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
	if (fd != -1)
		close(fd);
	return (fd);
}

int	builtins_input_redirection(t_redirect *redirect)
{
	int	fd;

	fd = -1;
	if (redirect->type == INPUT)
	{
		fd = open(redirect->filename, O_RDONLY);
		if (fd == -1)
			perror(redirect->filename);
		if (fd != -1)
			close(fd);
	}
	return (fd);
}

int	builtin_redirections(t_all *all)
{
	int	fd;
	int	i;

	if (!has_redir(all->redir))
		return (0);
	i = -1;
	while (all->redir[++i])
	{
		fd = -1;
		if (all->redir[i]->type == TRUNCATE || all->redir[i]->type == APPEND)
			fd = builtins_output_redirection(all->redir[i], all);
		else if (all->redir[i]->type == INPUT)
			fd = builtins_input_redirection(all->redir[i]);
		if (fd == -1)
		{
			all->exit_status = 1;
			break ;
		}
	}
	return (free_all_redir(all->redir), fd);
}

void	exec_non_forked(t_all *all, t_list *command_list)
{
	int	i;

	all->command = get_new_command(command_list, all);
	if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
		return ;
	all->redir = get_all_redirections(command_list, all);
	i = -1;
	if (all->redir)
		while (all->redir[++i])
			if (all->redir[i]->type == HEREDOC
				&& get_builtin_heredoc(all->redir[i]->filename, all))
				return (free_all_redir(all->redir));
	if (builtin_redirections(all) != -1)
		all->exit_status = builtin_execution(all->command, all);
	restore_std(all);
	return (free(all->command));
}

/*
	>: cat hello world << g
	heredoc> dsfsdf
	heredoc> g
	cat: hello: No such file or directory
	cat: world: No such file or directory
	>: echo hello world << g
	heredoc> dsfs
	heredoc> g
	hello world
	>: exit
	exit
*/