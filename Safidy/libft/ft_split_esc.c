#include "libft.h"

static char	*ft_escap_spliter(const char *s, char c)
{
	while (*s == c)
		s++;
	return ((char *)s);
}

static char	*ft_excape(const char *s, char c, size_t *count)
{
	char	quote;

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

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s != '\'' && *s != '\"')
			s++;
		s = ft_excape(s, c, &count);
	}
	return (count);
}

static size_t	ft_count_words_len(const char *s, char c)
{
	size_t	word_len;
	char	quote;

	word_len = 0;
	if (*s == '\'' || *s == '\"')
	{
		quote = *s++;
		while (s[word_len] && s[word_len] != quote)
			word_len++;
		if (s[word_len])
			word_len++;
		word_len++;
	}
	else
	{
		while (s[word_len] && s[word_len] != c)
			word_len++;
	}
	return (word_len);
}

static void	*ft_free_exit(char **tab)
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

char	**ft_split_esc(char const *s, char c)
{
	char	**tab;
	size_t	words;
	size_t	word_len;
	size_t	i;

	i = -1;
	words = ft_count_words(s, c);
	tab = (char **)calloc(sizeof(char *), (words + 1));
	if (!tab || words == 0)
		return (NULL);
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
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
