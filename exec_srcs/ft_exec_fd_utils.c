/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_fd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:43:30 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:13:36 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	dup_in(int fd[2], int closeall)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (closeall != 0)
		close(fd[1]);
}

void	dup_out(int fd[2], int closeall)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (closeall != 0)
		close(fd[0]);
}

void	restore_std(t_all *all)
{
	if (dup2(all->fd_og[0], STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdin failed\n");
	if (dup2(all->fd_og[1], STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdout failed\n");
}
