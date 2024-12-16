/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by safandri          #+#    #+#             */
/*   Updated: 2024/12/16 15:23:51 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/******************* Exec redirection file ******************/

char	*get_redir_name(char *command)
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

void	set_rd_name_type(t_redirect **redirection_files,
	char **command, int *type, t_all *all)
{
	int			i;
	int			j;

	i = -1;
	j = -1;
	while (command[++i])
	{
		if (type[i] == 1)
		{
			redirection_files[++j] = (t_redirect *)malloc(sizeof(t_redirect));
			if (!redirection_files[j])
				exec_error(NULL, all, "malloc error\n");
			redirection_files[j]->filename = get_redir_name(command[i]);
			if (command[i][0] == '>' && command[i][1] != '>' )
				redirection_files[j]->type = TRUNCATE;
			else if (command[i][0] == '>' && command[i][1] == '>' )
				redirection_files[j]->type = APPEND;
			else if (command[i][0] == '<' && command[i][1] != '<' )
				redirection_files[j]->type = INPUT;
			else if (command[i][0] == '<' && command[i][1] == '<' )
				redirection_files[j]->type = HEREDOC;
			if (!redirection_files[j]->filename)
				exec_error(NULL, all, "get_redirections\n");
		}
	}
}

t_redirect	**get_all_redirections(t_list *command_list, t_all *all)
{
	t_redirect	**redirection_files;
	char		**command;
	int			*type;
	int			count;
	int			i;

	command = (char **)command_list->content;
	type = (int *)command_list->type;
	count = 0;
	i = -1;
	while (command[++i])
		if (type[i] == 1)
			count++;
	if (count == 0)
		return (NULL);
	redirection_files = (t_redirect **)calloc(sizeof(t_redirect *), count + 1);
	if (!redirection_files)
		exec_error(NULL, all, "malloc error\n");
	set_rd_name_type(redirection_files, command, type, all);
	return (redirection_files);
}

int	handle_output_redirection(t_redirect *redirect,  t_all *all, t_redirect **redir_head)
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

int	handle_input_redirection(t_redirect *redirect, t_all *all, t_redirect **redir_head)
{
	int	fd;

	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
		fd_error(redirect->filename, redir_head, all);
	if (fd != -1 && dup2(fd, STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

void	free_all_redir(t_redirect **redir)
{
	int	i;

	i = -1;
	if (redir)
	{
		while (redir[++i])
		{
			ft_free(redir[i]->filename);
			free(redir[i]);
		}
		free(redir);
	}
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
			fd = handle_heredoc_redirection(redir[i]->fd);
		if (fd != -1)
			close(fd);
	}
	return (free_all_redir(redir), fd);
}
