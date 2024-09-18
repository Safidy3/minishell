/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <larakoto@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:57:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/06 16:53:10 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

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
char **parse_and_add_command(t_command_list **commande, char *line)
{
    int i;
    i = 0;
    char **token;
    token = ft_split(line); 

    while (token[i])
    {
        ft_lstadd_back(commande,ft_lstnew(token[i]));
        i++;
    }
	return(token);
    
}
void ft_print_list(t_command_list cmd)
{
	while (cmd.next)
	{
		printf("%s", cmd.value);
	}
	
}



int main(int argc, char **argv, char **envp)
{
    char *line;
	int i = 0;
	char **commande;
    // t_command_list *commande = NULL;

	char *path = NULL;
	line = readline("$> ");
	// parse_and_add_command(&commande, line);
	char **directory;
	char *fu = getenv("PATH");
	directory = (ft_split_simple(fu,':'));
	commande = ft_split(line);
		while (directory[i])
		{
			path = ft_strjoin(directory[i],"/");
			path = ft_strjoin(path,line);
			if (access(path,F_OK) == 0)
			{
				execve(path,commande,envp);
				// printf("%s\n",path);
			}
			// argv = { "ls", "-l", NULL};
			i++;
		}			
		free(line);
    return 0;
}

