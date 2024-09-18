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
#include <stdio.h>
#include <stdlib.h>

bool	is_separator(char c)
{
	return (c == '|' || c == ' ' || c == '<' || c == '>');
}

static size_t	count_words(char const *s)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (is_separator(s[i]) && s[i] != ' ')
		{
			count++;
			i++;
		}
        else
        {
            count++;
            while (s[i] && !is_separator(s[i]))
                i++;
        }
	}
	return (count);
}

static void	fill_word(char *new, char const *s, size_t start, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		new[i] = s[start + i];
		i++;
	}
	new[i] = '\0';
}

static void	split_words(char **tab, char const *s)
{
	size_t	index;
	size_t	i;
	size_t	word_start;

	index = 0;
	i = 0;
	word_start = 0;
	while (s[index])
	{

		while (s[index] == ' ')
			index++;
		word_start = index;
		if (is_separator(s[index]) && s[index] != ' ')
		{
			tab[i] = malloc(2 * sizeof(char));
			tab[i][0] = s[index];
			tab[i][1] = '\0';
			i++;
			index++;
		}
		else
		{
			while (s[index] && !is_separator(s[index]))
				index++;
			if (index > word_start)
			{
				tab[i] = malloc(sizeof(char) * (index - word_start + 1));
				fill_word(tab[i], s, word_start, index - word_start);
				i++;
			}
		}
	}
	tab[i] = NULL;
}

char	**ft_split(char *s)
{
	size_t	w;
	char	**tab;

	w = count_words(s);
	tab = malloc(sizeof(char *) * (w + 1));
	if (!tab)
		return (NULL);
	split_words(tab, s);
	return (tab);
}
