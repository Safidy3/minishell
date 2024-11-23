/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:28:22 by larakoto          #+#    #+#             */
/*   Updated: 2024/11/23 14:24:53 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/************************************************** */

void	ft_swap_string(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void	ft_sort_list(t_env_list *env)
{
	t_env_list	*tmp;

	tmp = env;
	while (tmp->next)
	{
		if (strcmp(tmp->value, tmp->next->value) > 0)
		{
			ft_swap_string(&tmp->first, &tmp->next->first);
			ft_swap_string(&tmp->second, &tmp->next->second);
			ft_swap_string(&tmp->value, &tmp->next->value);
			tmp = env;
		}
		else
			tmp = tmp->next;
	}
}

void	join_first_second(int flag, t_env_list *new)
{
	char	*tmp;

	if (flag == 1)
	{
		free(new->value);
		tmp = ft_strjoin(new->first, "=");
		if (new->second)
			new->value = ft_strjoin(tmp, new->second);
		else
			new->value = ft_strdup(new->first);
		free(tmp);
	}
}

t_env_list	*ft_lstnew(char *content, int flag)
{
	t_env_list	*new;
	char		*line;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->value = strdup(content);
	line = strchr(new->value, '=');
	if (line)
	{
		if (flag == 1)
			new->first = ft_substr(new->value, 0, (line - new->value - 1));
		else
			new->first = ft_substr(new->value, 0, (line - new->value));
		new->second = strdup(++line);
	}
	else
	{
		new->first = strdup(content);
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

/************************************************** */

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

void	int_lst_env(t_env_list **list, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_lstadd_back(list, ft_lstnew(envp[i], 0));
		i++;
	}
}

char	**list_to_array(t_env_list *env)
{
	int			i;
	char		**dir;
	char		*cha_join;
	t_env_list	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	dir = (char **)calloc(sizeof(char *), (i + 1));
	i = -1;
	while (env)
	{
		cha_join = ft_strjoin((env->first), "=");
		dir[++i] = ft_strjoin(cha_join, env->second);
		free(cha_join);
		env = env->next;
	}
	return (dir);
}

void	ft_print_env(t_env_list *env)
{
	t_env_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->second)
			printf("%s=%s\n", tmp->first, tmp->second);
		tmp = tmp->next;
	}
}
