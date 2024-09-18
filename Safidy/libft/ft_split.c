/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 14:52:29 by safandri          #+#    #+#             */
/*   Updated: 2024/06/05 16:16:12 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_escap_spliter(const char *s, char splitter)
{
	while (*s == splitter)
		s++;
	return ((char *)s);
}

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s != '\0')
			s++;
		if (*s != c && *s != '\0')
		{
			count++;
			while (*s != c && *s != '\0')
				s++;
		}
	}
	return (count);
}

static size_t	ft_count_words_len(const char *s, char c)
{
	size_t	word_len;

	s = ft_escap_spliter(s, c);
	word_len = 0;
	while (s[word_len] != c && s[word_len] != '\0')
		word_len++;
	return (word_len);
}

static void	ft_free_all(char **tab)
{
	int	i;

	i = -1;
	if (tab)
	{
		while (tab[++i])
			free(tab[i]);
		free(tab);
	}
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**tab;
	size_t	word_len;
	size_t	i;

	words = ft_count_words(s, c);
	i = -1;
	tab = (char **)malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
		word_len = ft_count_words_len(s, c);
		tab[i] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!tab[i])
		{
			ft_free_all(tab);
			return (NULL);
		}
		ft_strlcpy(tab[i], s, word_len + 1);
		s += word_len;
	}
	tab[words] = NULL;
	return (tab);
}
