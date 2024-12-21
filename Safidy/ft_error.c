/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:26:11 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 15:44:03 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fd_error(char *file_path, t_redirect **redir, t_all *all)
{
	ft_putstr_fd("bash: ", 2);
	perror(file_path);
	free_all_redir(redir);
	ft_free(all->bin_path);
	ft_free(all->command);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(EXIT_FAILURE);
}

void	exec_error(char *bin_path, t_all *all, char *msg)
{
	ft_free(bin_path);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	if (msg)
	{
		ft_putstr_fd("Exec err: ", 2);
		ft_putstr_fd(msg, 2);
	}
	exit(EXIT_FAILURE);
}

int	command_not_found_point(t_all *all)
{
	ft_putstr_fd("bash: line 1: ", 2);
	ft_putstr_fd(all->command[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_all_redir(all->redir);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	ft_free(all->command);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(2);
}

int	command_not_found(t_all *all)
{
	ft_putstr_fd("bash: line 1: ", 2);
	ft_putstr_fd(all->command[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_all_redir(all->redir);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	ft_free(all->command);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(127);
}
