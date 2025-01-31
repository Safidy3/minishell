/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_get_set.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:21:36 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:28:44 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	set_type_fd_heredoc(t_redirect *redirection_files)
{
	redirection_files->type = HEREDOC;
	redirection_files->fd = -1;
}

void	set_rd_name_type(t_redirect **redirection_files,
	char **command, int *type, t_all *all)
{
	int	i;
	int	j;

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
				set_type_fd_heredoc(redirection_files[j]);
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
	redirection_files = (t_redirect **)ft_calloc(sizeof(t_redirect *),
			count + 1);
	if (!redirection_files)
		exec_error(NULL, all, "malloc error\n");
	set_rd_name_type(redirection_files, command, type, all);
	return (redirection_files);
}

void	free_all_redir(t_redirect **redir)
{
	int	i;

	i = -1;
	if (redir)
	{
		while (redir[++i])
		{
			if (redir[i]->type == HEREDOC && redir[i]->fd != -1)
				close(redir[i]->fd);
			ft_free(redir[i]->filename);
			free(redir[i]);
		}
		free(redir);
	}
}
