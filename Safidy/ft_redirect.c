/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 15:38:16 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/******************* Exec redirection file ******************/

char	*get_redirection_file_name(char *command)
{
	char	*redirection_file;
	int		i;

	i = 0;
	while (command[i] && (command[i] == '>' || command[i] == '<'
			|| ft_isspace(command[i])))
		i++;
	redirection_file = ft_strdup(&command[i]);
	if (!redirection_file)
		return (NULL);
	return (redirection_file);
}

t_redirect    **get_all_redirections(char **command, t_all *all)
{
	t_redirect	**redirection_files;
	int			count;
	int			i;
	int			j;

	count = 0;
	i = -1;
	while (command[++i])
		if (command[i][0] == '>' || command[i][0] == '<')
			count++;
	if (count == 0)
		return (NULL);
	redirection_files = (t_redirect **)calloc(sizeof(t_redirect *), count + 1);
	if (!redirection_files)
		exec_error(NULL, all, "malloc error\n");
	i = -1;
	j = -1;
	while (command[++i])
	{
		if (command[i][0] == '>' || command[i][0] == '<')
		{
			redirection_files[++j] = (t_redirect *)malloc(sizeof(t_redirect));
			if (!redirection_files[j])
				exec_error(NULL, all, "malloc error\n");
			redirection_files[j]->filename = get_redirection_file_name(command[i]);
			if (command[i][0] == '>')
				redirection_files[j]->type = (command[i][1] == '>') ? APPEND : TRUNCATE;
			else
				redirection_files[j]->type = (command[i][1] == '<') ? HEREDOC : INPUT;
			if (!redirection_files[j]->filename)
				exec_error(NULL, all, "get_redirections\n");
		}
	}
	return (redirection_files);
}

int handle_output_redirection(t_redirect *redirect, t_all *all)
{
	int fd;

	fd = 0;
	if (redirect->type == TRUNCATE)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		fd_error(redirect->filename);
	if (fd != -1 && dup2(fd, STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int	handle_input_redirection(t_redirect *redirect, t_all *all)
{
	int	fd;

	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
		fd_error(redirect->filename);
	if (fd != -1 && dup2(fd, STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

int manage_redirections(t_redirect **redirections, t_all *all)
{
	int fd;
	int i;

	if (!redirections)
		return (0);
	i = -1;
	while (redirections[++i])
		if (redirections[i]->type == HEREDOC)
			if (get_heredoc(redirections[i]->filename, &redirections[i]->fd, all))
				return (1);
	i = -1;
	while (redirections[++i])
	{
		fd = -1;
		if (redirections[i]->type == TRUNCATE || redirections[i]->type == APPEND)
			fd = handle_output_redirection(redirections[i], all);
		else if (redirections[i]->type == INPUT)
			fd = handle_input_redirection(redirections[i], all);
		else if (redirections[i]->type == HEREDOC)
			handle_heredoc_redirection(redirections[i]->fd);
		if (fd != -1)
			close(fd);
		ft_free(redirections[i]->filename);
		free(redirections[i]);
		if (fd == -1)
		{
			dup2(all->fd_og[0], STDIN_FILENO);
			dup2(all->fd_og[1], STDOUT_FILENO);
			break;
		}
	}
	free(redirections);
	return (fd);
}
