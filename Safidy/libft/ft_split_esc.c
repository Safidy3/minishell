
#include "libft.h"

static char *ft_escap_spliter(const char *s, char c)
{
	while (*s == c)
		s++;
	return ((char *)s);
}

static char *ft_excape(const char *s, char c, size_t *count)
{
	char quote;

	if (*s == '\'' || *s == '\"')
	{
		quote = *s++;
		(*count)++;
		while (*s && *s != quote)
			s++;
		if (*s)
			s++;
	}
	else if (*s && *s != c)
	{
		(*count)++;
		while (*s && *s != c)
			s++;
	}
	return ((char *)s);
}

static size_t ft_count_words(const char *s, char c)
{
	size_t count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s != '\'' && *s != '\"')
			s++;
		s = ft_excape(s, c, &count);
	}
	return (count);
}

static size_t ft_count_words_len(const char *s, char c)
{
	size_t word_len = 0;
	char quote;

	if (*s == '\'' || *s == '\"')
	{
		quote = *s++;
		while (s[word_len] && s[word_len] != quote)
			word_len++;
		if (s[word_len]) // Include quotes in length
			word_len++;
		word_len++; // For opening quote
	}
	else
	{
		while (s[word_len] && s[word_len] != c)
			word_len++;
	}
	return word_len;
}

static void ft_free_all(char **tab)
{
	int i;

	i = -1;
	if (tab)
	{
		while (tab[++i])
			free(tab[i]);
		free(tab);
	}
}

char **ft_split_esc(char const *s, char c)
{
	size_t words;
	char **tab;
	size_t word_len;
	size_t i;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	tab = (char **)malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (i < words)
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
		if (*s)
			s++; // Skip delimiter
		i++;
	}
	tab[words] = NULL;
	return (tab);
}

