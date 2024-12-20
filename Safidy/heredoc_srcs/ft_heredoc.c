/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:50:53 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 14:36:22 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*new_delimiter(char *old, int *del_quote)
{
	char	*new;
	int		i;
	int		j;
	char	quote;

	new = NULL;
	i = 0;
	j = 0;
	if (old[i] != '\'' && old[i] != '"')
		return (old);
	*del_quote = 1;
	quote = old[i];
	while (old[++i] && old[i] != quote)
		j++;
	new = (char *)ft_calloc(sizeof(char), (j + 1));
	i = 0;
	j = 0;
	while (old[++i] && old[i] != quote)
		new[j++] = old[i];
	return (new);
}

void	handle_hered_singint(t_all *all, int pipe_fd[2], char *buffer)
{
	dup2(all->fd_og[0], STDIN_FILENO);
	ft_free(buffer);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	free_all_redir(all->redir);
	g_flag = 0;
}

int	handle_ctrl_d(char *delimiter, char *input)
{
	if (!input && g_flag != SIGINT)
	{
		ft_putstr_fd("bash: warning: here-document (", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd(")\n", 2);
		return (1);
	}
	return (0);
}

char	*read_join_heredoc(char *buffer, char *delimiter,
	int pipe_fd[2], t_all *all)
{
	char	*input;
	int		replace_env;

	input = NULL;
	replace_env = 0;
	delimiter = new_delimiter(delimiter, &replace_env);
	while (1)
	{
		put_signal_handlig(2);
		input = readline("heredoc> ");
		if (handle_ctrl_d(delimiter, input))
			break ;
		if (g_flag == SIGINT)
			return (handle_hered_singint(all, pipe_fd, buffer), NULL);
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		buffer = join_result(buffer, input, replace_env, all);
	}
	if (replace_env == 1)
		free(delimiter);
	return (buffer);
}

int	get_heredoc(char *delimiter, int *fd, t_all *all)
{
	char	*buffer;
	int		pipe_fds[2];

	buffer = ft_strdup("");
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), -1);
	buffer = read_join_heredoc(buffer, delimiter, pipe_fds, all);
	if (buffer)
	{
		write(pipe_fds[1], buffer, ft_strlen(buffer));
		close(pipe_fds[1]);
		*fd = dup(pipe_fds[0]);
		close(pipe_fds[0]);
		free(buffer);
		return (0);
	}
	return (1);
}
