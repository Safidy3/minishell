/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_forked.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:11:26 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:11:43 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
