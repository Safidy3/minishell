/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_separator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:55:10 by larakoto          #+#    #+#             */
/*   Updated: 2024/09/21 09:55:09 by larakoto         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

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
				tab[i] = malloc(sizeof(char) * ((index - word_start) + 1));
				fill_word(tab[i], s, word_start, (index - word_start));
				i++;
			}
		}
	}
	tab[i] = NULL;
}

char	**ft_split_separator(char *s)
{
	size_t	nbr_word;
	char	**tab;

	nbr_word = count_words(s); 
	tab = malloc(sizeof(char *) * (nbr_word + 1));
	if (!tab)
		return (NULL);
	split_words(tab, s);
	return (tab);
}
