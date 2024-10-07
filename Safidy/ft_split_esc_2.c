#include "minishell.h"

static char	*ft_escap_spliter(const char *s, char c)
{
	while (*s == c)
		s++;
	return ((char *)s);
}

static char	*ft_escape_quote(const char *s)
{
	char	quote;
	
	quote = *s++;
	while (*s && *s != quote)
		s++;
	if (*s)
		s++;
	return ((char *) s);
}

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		s = ft_escap_spliter(s, c);
		if (*s && *s != c)
		{
			count++;
			while (*s && *s != c)
			{
				if (*s == '\'' || *s == '\"')
					s = ft_escape_quote(s);
				if (*s && *s != c)
					s++;
			}
		}
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

static char	*cpy_to_arr(char const *s, char **tab, int word_len, int i)
{
	ft_strlcpy(tab[i], s, word_len + 1);
	s += word_len;
	if (*s)
		s++;
	return ((char *)s);
}

char	**ft_split_esc_2(char const *s, char c)
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
		s = cpy_to_arr(s, tab, word_len, i);
	}
	return (tab);
}
