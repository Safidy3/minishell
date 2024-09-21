/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:57:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/21 16:11:02 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

void	parse_and_add_command(t_command_list **commande, char *line)
{
	int		i;
	char	**token;

	i = 0;
	token = ft_split_separator(line);
	while (token[i])
	{
		ft_lstadd_back(commande, ft_lstnew(token[i]));
		i++;
	}
}
void	ft_execute(char *line, char **envp)
{
	char	**directory;
	int		i;
	char	*path;
	char	*env;
	char	**commande;

	path = NULL;
	i = 0;
	env = getenv("PATH");
	directory = (ft_split_simple(env, ':'));
	commande = ft_split_separator(line);
	while (directory[i])
	{
		path = ft_strjoin(directory[i], "/");
		path = ft_strjoin(path, commande[0]);
		if (access(path, F_OK | X_OK) == 0)
		{
			execve(path, commande, envp);
			exit(1);
		}
		i++;
	}
}

// void	ft_print_list(t_command_list *command)
// {
// 	while (command)
// 	{
// 		printf("%s\n", command->value);
// 		command = command->next;
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	int				i;
	t_command_list	*command;
	int				pid;

	command = NULL;
	i = 0;
	put_signal_handlig();
	while (1)
	{
		line = readline("$> ");
		if (!line)
		{
			exit(0);
		}
		ft_exit(line);
		add_history(line);
		parse_and_add_command(&command, line);
		pid = fork();
		if (pid == 0)
			ft_execute(line, envp);
		else
			waitpid(pid, NULL, 0);
	}
	return (0);
}
  