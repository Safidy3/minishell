/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_geter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:01:48 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 15:45:39 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	get_command_bin(t_all *all, t_list *command_list)
{
	all->command = get_new_command(command_list, all);
	if (all->command)
	{
		if (!all->command[0][0])
			command_not_found(all);
		if (!ft_strcmp(all->command[0], "."))
			command_not_found_point(all);
		if (ft_strchr(all->command[0], '/') && is_dir(all->command[0], all))
			command_dir_error(all);
		all->bin_path = get_bin_path(all->command[0], all);
	}
	if (!all->bin_path && all->command && !is_builtins(all->command[0]))
		command_not_found(all);
	if (all->redir)
		manage_redirections(all->redir, all);
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
