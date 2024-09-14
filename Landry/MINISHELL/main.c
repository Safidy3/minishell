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
    int i;
    i = 0;
    char **token;
    token = ft_split(line,' '); 

    while (token[i])
    {
        ft_lstadd_back(commande,ft_lstnew(token[i]));
        i++;
    }
    
}
int main(void)
{
    char *line;
    t_command_list *commande = NULL;
    while (1)
    {
        line = readline("$> ");
        if (!line)
            break;
        add_history(line);
        parse_and_add_command(&commande, line);
        free(line);
        t_command_list *tmp = commande;
        while (tmp)
        {
            printf("Argument: %s\n", tmp->value);
            tmp = tmp->next;
        }
    }
    return 0;
}


// int main(int argc,char **argv)
// {
//     char *line;
//     char    **envp;
//     int i= 1;
//     t_command_list *comamnde = NULL;
//     while( i < argc)
//     {
//         ft_lstadd_back(&comamnde,ft_lstnew(argv[i]));
//         i++;
//     }
//     t_command_list *tmp = comamnde;
//     while (tmp)
//     {
//         printf("Argument: %s\n", tmp->value);
//         tmp = tmp->next;
//     }
//     while (1)
//     {
//         line = readline("$>");
//         add_history(line);   
//         free(line);
//     }
//     return 0;
// }
