/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:57:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/20 17:22:50 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

t_command_list	*ft_lstnew(char *content)
{
	t_command_list	*new;

	new = (t_command_list *)malloc(sizeof(t_command_list));
	if (!new)
		return (NULL);
	new->value = content;
	new->next = NULL;
	return (new);
}
t_command_list	*ft_lstlast(t_command_list *lst)
{
	t_command_list	*tmp;

	while ((lst->next) != NULL)
		lst = lst->next;
	tmp = lst;
	return (tmp);
}

void	ft_lstadd_back(t_command_list **lst, t_command_list *new_element)
{
	t_command_list	*last;

	if (*lst == NULL)
	{
		*lst = new_element;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_element;
}
void parse_and_add_command(t_command_list **commande, char *line)
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
void	ft_execute(char *line,  char **envp)
{
	char	**directory;
	int		i;
	char	*path;
	char	*env;

	char **commande;
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


void	ft_print_list(t_command_list *command)
{
	while (command)
	{
		printf("%s\n", command->value);
		command = command->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	int				i;
	t_command_list	*command;
	int pid;
	command = NULL;
	i = 0;
	while (1)
	{
		line = readline("$> ");
		ft_exit(line);
		add_history(line);
		parse_and_add_command(&command, line);
		pid = fork();
		if (pid == 0)
		{
			ft_execute(line,envp);
		}
		else
		waitpid(pid,NULL,0);
		
	}
	return (0);
}
