/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:26:15 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_output_redirection(t_redirect *redirect,
	t_all *all, t_redirect **redir_head)
{
	int	fd;

	fd = 0;
	if (redirect->type == TRUNCATE)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		fd_error(redirect->filename, redir_head, all);
	if (fd != -1 && dup2(fd, STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int	handle_input_redirection(t_redirect *redirect,
	t_all *all, t_redirect **redir_head)
{
	int	fd;

	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
		fd_error(redirect->filename, redir_head, all);
	if (fd != -1 && dup2(fd, STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int	handle_heredoc_redirection(int fd)
{
	printf("*%d  ", fd);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	return (-2);
}

int	manage_redirections(t_redirect	**redir, t_all *all)
{
	int	fd;
	int	i;

	i = -1;
	while (redir[++i])
	{
		fd = -1;
		if (redir[i]->type == TRUNCATE || redir[i]->type == APPEND)
			fd = handle_output_redirection(redir[i], all, redir);
		else if (redir[i]->type == INPUT)
			fd = handle_input_redirection(redir[i], all, redir);
		else if (redir[i]->type == HEREDOC)
		{
			fd = handle_heredoc_redirection(redir[i]->fd);
			printf(".%d\n", fd);
		}
		if (fd != -1 && fd != -2)
			close(fd);
	}
	return (free_all_redir(redir), fd);
}
