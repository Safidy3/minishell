/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:50:53 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 15:00:24 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc_redirection(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return;
	}
}

char *read_join_heredoc(char *buffer, char *delimiter, t_all *all)
{
	char *input;
	char *tmp;
	char *tmp2;
	(void)all;

	tmp2 = NULL;
	tmp = NULL;
	input = NULL;
	if (!buffer)
		buffer = ft_strdup("");
	while (1)
	{
		put_signal_handlig(2);
		input = readline("heredoc> ");
		if (!input && flag != SIGINT)
		{
			write(2,"bash: warning: here-document (",30);
			write(2,delimiter,ft_strlen(delimiter));
			write(2,")\n",2);
			break;
		}
		if (flag == SIGINT)
		{
			dup2(all->fd_og[0], STDIN_FILENO);
			flag = 0;
			return (NULL);
		}
		if (strcmp(input, delimiter) == 0)
		{
			if (!buffer)
				buffer = ft_strdup("");
			free(input);
			break;
		}
		tmp = ft_strjoin(input, "\n");
		free(input);
		tmp2 = buffer;
		buffer = ft_strjoin(buffer, tmp);
		free(tmp2);
		free(tmp);
	}
	return (buffer);
}

int get_heredoc(char *delimiter, int *fd, t_all *all)
{
	char *buffer = NULL;

	int pipe_fds[2];
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	buffer = read_join_heredoc(buffer, delimiter, all);
	if (buffer)
	{
		write(pipe_fds[1], buffer, strlen(buffer));
		close(pipe_fds[1]);
		*fd = dup(pipe_fds[0]);
		close(pipe_fds[0]);
		free(buffer);
		return (0);
	}
	else
		return (1);
	return (0);
}
