/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:18:05 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 16:30:18 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_escape_quote(char *s)
{
	char	quote;

	quote = *s++;
	while (*s && *s != quote)
		s++;
	if (*s)
		s++;
	return (s);
}

void	*ft_free_exit(char **tab)
{
	int	i;

	i = -1;
	if (tab)
	{
		while (tab[++i])
			free(tab[i]);
		free(tab);
	}
	return (NULL);
}
