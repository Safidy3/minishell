#include "minishell.h"

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

static int	is_special_char(char c)
{
	return (c == '<' || c == '>');
}

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

static const char *ft_escape_special_char(const char *s)
{
	if (is_special_char(*s))
	{
		s++;
		if (is_special_char(*s))
			s++;
		while (*s == ' ')
			s++;
		while (*s && *s != ' ' && !is_special_char(*s))
			s++;
	}
	return (s);
}

static size_t	ft_count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		s = ft_escap_spliter(s, c);
		if (*s && !is_special_char(*s))
		{
			count++;
			while (*s && *s != c && !is_special_char(*s))
			{
				if (*s == '\'' || *s == '\"')
					s = ft_escape_quote(s);
				if (*s && *s != c && !is_special_char(*s))
					s++;
			}
		}
		else if (*s && is_special_char(*s))
		{
			count++;
			s = ft_escape_special_char(s);
		}
	}
	return (count);
}

static void	skip_quote(const char *s, int *word_len, int *i)
{
	char quote;

	if (s[*i] == '\'' || s[*i] == '"')
	{
		quote = s[(*i)++];
		while (s[*i] && s[*i] != quote)
			(*i)++;
		if (s[*i])
			(*i)++;
		*word_len -= 2;
	}
}

static void handle_special_char(const char *s, int *word_len, int *i)
{
	(*i)++;
	if (is_special_char(s[*i]))
		(*i)++;
	while (s[*i] == ' ')
		(*i)++;
	while (s[*i] && s[*i] != ' ' && !is_special_char(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '"')
			skip_quote(s, word_len, i);
		else if (s[*i])
			(*i)++;
	}
}

static size_t	ft_count_words_len(const char *s, char c)
{
	char	quote;
	int		word_len;
	int		i;

	word_len = 0;
	i = 0;
	if (s[i] && s[i] != c && !is_special_char(s[i]))
	{
		while (s[i] && s[i] != c && !is_special_char(s[i]))
		{
			if (s[i] == '\'' || s[i] == '"')
				skip_quote(s, &word_len, &i);
			else if (s[i] && s[i] != c && !is_special_char(s[i]))
				i++;
			else if (s[i] == c || is_special_char(s[i]))
				break;
		}
	}
	else if (is_special_char(s[i]))
		handle_special_char(s, &word_len, &i);
	word_len += i;
	return (word_len);
}

static char	*cpy_to_arr(char const *s, char *tab, int word_len)
{
	char	quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (j < word_len)
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				tab[j++] = s[i++];
			if (s[i])
				i++;
		}
		else
			tab[j++] = s[i++];
	}
	s += i;
	// if (*s)
	// 	s++;
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
	word_len = ft_count_words_len(s, c);
	tab[i] = (char *)calloc(sizeof(char), (word_len + 1));
	if (!tab[i])
		return (ft_free_exit(tab));
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
		word_len = ft_count_words_len(s, c);
		tab[i] = (char *)calloc(sizeof(char), (word_len + 1));
		if (!tab[i])
			return (ft_free_exit(tab));
		s = cpy_to_arr(s, tab[i], word_len);
		printf("tab[i] = %s : %zu\n", tab[i], word_len);
	}
	return (tab);
}
