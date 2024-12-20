/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:53:53 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 11:57:27 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_swap_string(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

t_env_list	*ft_lstnew(char *content, int flag)
{
	t_env_list	*new;
	char		*line;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->value = ft_strdup(content);
	line = ft_strchr(new->value, '=');
	if (line)
	{
		if (flag == 1)
			new->first = ft_substr(new->value, 0, (line - new->value - 1));
		else
			new->first = ft_substr(new->value, 0, (line - new->value));
		new->second = ft_strdup(++line);
	}
	else
	{
		new->first = ft_strdup(content);
		new->second = NULL;
	}
	join_first_second(flag, new);
	new->next = NULL;
	return (new);
}

t_env_list	*ft_lstlast(t_env_list *lst)
{
	t_env_list	*tmp;

	while ((lst->next) != NULL)
		lst = lst->next;
	tmp = lst;
	return (tmp);
}

void	ft_lstadd_back(t_env_list **lst, t_env_list *new_element)
{
	t_env_list	*last;

	if (*lst == NULL)
	{
		*lst = new_element;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_element;
}

void	ft_free_env_list(t_env_list *env)
{
	t_env_list	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->first);
		free(tmp->second);
		free(tmp->value);
		free(tmp);
	}
}
