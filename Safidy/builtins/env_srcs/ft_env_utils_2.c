/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:55:12 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 14:59:38 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env_list	*ft_lstnew_init(char *content, int flag)
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
	free_shlvl(content);
	join_first_second(flag, new);
	new->next = NULL;
	return (new);
}

void	free_shlvl(char *content)
{
	if (!ft_strncmp(content, "SHLVL", 5))
		free(content);
}
