/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_esc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:14:24 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 16:30:00 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*set_last_pipe(const char *s, int *last_was_pipe)
{
	*last_was_pipe = 1;
	return ((char *)++s);
}

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;
	int		last_was_pipe;

	count = 0;
	last_was_pipe = 0;
	while (*s)
	{
		if (*s && *s != c)
		{
			count++;
			last_was_pipe = 0;
			while (*s && *s != c)
			{
				if (*s == '\'' || *s == '\"')
					s = ft_escape_quote((char *)s);
				if (*s && *s != c)
					s++;
			}
		}
		else if (*s == c)
			s = set_last_pipe(s, &last_was_pipe);
	}
	return (count);
}

static size_t	ft_count_words_len(const char *s, char c)
{
	size_t	word_len;
	char	quote;

	word_len = 0;
	while (s[word_len] && s[word_len] != c)
	{
		if (s[word_len] == '\'' || s[word_len] == '"')
		{
			quote = s[word_len];
			word_len++;
			while (s[word_len] && s[word_len] != quote)
				word_len++;
			if (s[word_len])
				word_len++;
		}
		if (s[word_len] && s[word_len] != c)
			word_len++;
	}
	return (word_len);
}

char	**ft_split_esc(const char *s, char c)
{
	char	**tab;
	size_t	words;
	size_t	word_len;
	size_t	i;

	i = -1;
	words = ft_count_words(s, c);
	tab = (char **)ft_calloc(sizeof(char *), (words + 1));
	if (!tab || words == 0)
		return (NULL);
	while (++i < words)
	{
		while (*s == c)
			s++;
		word_len = ft_count_words_len(s, c);
		tab[i] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!tab[i])
			return (ft_free_exit(tab));
		ft_strlcpy(tab[i], s, word_len + 1);
		s += word_len;
		if (*s)
			s++;
	}
	return (tab);
}
