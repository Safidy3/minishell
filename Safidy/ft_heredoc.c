/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:50:53 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/12 16:23:51 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_redirection(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	return (10);
}

char	*join_result(char *buffer, char *input)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	tmp = ft_strjoin(input, "\n");
	free(input);
	tmp2 = buffer;
	buffer = ft_strjoin(buffer, tmp);
	return (free(tmp2), free(tmp), buffer);
}

int	herdoc_delimiter(char *input, char *delimiter)
{
	if (strcmp(input, delimiter) == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

char	*read_join_heredoc(char *buffer, char *delimiter, t_all *all)
{
	char	*input;

	input = NULL;
	while (1)
	{
		put_signal_handlig(2);
		input = readline("heredoc> ");
		if (!input && flag != SIGINT)
		{
			write(2, "bash: warning: here-document (", 30);
			write(2, delimiter, ft_strlen(delimiter));
			write(2, ")\n", 2);
			break ;
		}
		if (flag == SIGINT)
		{
			dup2(all->fd_og[0], STDIN_FILENO);
			flag = 0;
			return (NULL);
		}
		if (herdoc_delimiter(input, delimiter))
			break ;
		buffer = join_result(buffer, input);
	}
	return (buffer);
}

int	get_heredoc(char *delimiter, int *fd, t_all *all)
{
	char	*buffer;
	int		pipe_fds[2];

	buffer = ft_strdup("");
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
	return (1);
}
