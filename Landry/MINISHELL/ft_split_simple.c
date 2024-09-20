/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto <larakoto@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:55:10 by larakoto          #+#    #+#             */
/*   Updated: 2024/07/31 17:49:16 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static size_t	count(char const *s, char c)
{
	size_t	count;
	size_t	a;

	count = 0;
	a = 0;
	while (s[a])
	{
		if (s[a] == c)
			count++;
		a++;
	}
	if (s[a - 1] != '\n')
		count++;
	return (count);
}

static void	fill(char *new, char const *s, char c)
{
	size_t	a;

	a = 0;
	while (s[a] && s[a] != c)
	{
		new[a] = s[a];
		a++;
	}
	new[a] = '\0';
}

static void	split(char **tab, char const *s, char c)
{
	size_t	count;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	while (s[index])
	{
		count = 0;
		while (s[index + count] && s[index + count] != c)
			count++;
		if (s[index + count] == c)
			count++;
		if (count > 0)
		{
			tab[i] = malloc(sizeof(char) * (count + 1));
			fill(tab[i], (s + index), c);
			i++;
			index = index + count;
		}
		else
			index++;
	}
	tab[i] = 0;
}

char	**ft_split_simple(char const *s, char c)
{
	size_t	w;
	char	**tab;

	w = count(s, c);
	tab = malloc(sizeof(char *) * (w + 1));
	if (!tab)
		return (NULL);
	split(tab, s, c);
	return (tab);
}
