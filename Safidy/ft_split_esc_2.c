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

static char	*ft_escap_spliter(char *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}

static char	*ft_escape_quote(char *s)
{
	char	quote;

	quote = *s++;
	while (*s && *s != quote)
		s++;
	if (*s)
		s++;
	return (s);
}

static char	*ft_escape_special_char(char *s)
{
	if (is_special_char(*s))
	{
		s++;
		if (is_special_char(*s))
			s++;
		while (ft_isspace(*s))
			s++;
		if (*s && (*s == '\'' || *s == '"'))
			s = ft_escape_quote(s);
		while (*s && !ft_isspace(*s) && !is_special_char(*s))
			s++;
	}
	return (s);
}

static size_t	ft_count_words(char *s, char c)
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

static void	skip_quote(char *s, int *word_len, int *i)
{
	char	quote;

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

static void	handle_special_char(char *s, int *word_len, int *i)
{
	(*i)++;
	if (is_special_char(s[*i]))
		(*i)++;
	while (ft_isspace(s[*i]))
		(*i)++;
	while (s[*i] && !ft_isspace(s[*i]) && !is_special_char(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '"')
			skip_quote(s, word_len, i);
		else if (s[*i])
			(*i)++;
	}
}

static size_t	ft_count_words_len(char *s, char c)
{
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
				break ;
		}
	}
	else if (is_special_char(s[i]))
		handle_special_char(s, &word_len, &i);
	word_len += i;
	return (word_len);
}

static char	*cpy_to_arr(char *s, char *tab, int word_len)
{
	char	quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (word_len == 0)
		return (s + 2);
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
	return (s);
}

char	**ft_split_esc_2(char *s, char c)
{
	char	**tab;
	size_t	words;
	size_t	word_len;
	size_t	i;

	i = -1;
	words = ft_count_words(s, c);
	tab = (char **)calloc(sizeof(char *), (words + 1));
	if (!tab || words == 0 || !s)
		return (NULL);
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
		word_len = ft_count_words_len(s, c);
		tab[i] = (char *)calloc(sizeof(char), (word_len + 1));
		if (!tab[i])
			return (ft_free_exit(tab));
		s = cpy_to_arr(s, tab[i], word_len);
	}
	return (tab);
}

/****************** ARG TYPE  *****************/

static int	arg_skip_quote(char *s)
{
	char	quote;
	int		i;

	i = -1;
	if (s[++i] == '\'' || s[i] == '"')
	{
		quote = s[i++];
		while (s[i] && s[i] != quote)
			i++;
		if (s[i])
			i++;
	}
	return (i);
}

static int	arg_special_char(char *s)
{
	int	i;

	i = 0;
	if (is_special_char(s[i]))
		i++;
	if (s[i - 1] == s[i])
		i++;
	while (ft_isspace(s[i]))
		i++;
	while (s[i] && !ft_isspace(s[i]) && !is_special_char(s[i]))
	{
		if (s[i] == '\'' || s[i] == '"')
			i += arg_skip_quote(&s[i]);
		else if (s[i])
			i++;
	}
	return (i);
}

static size_t	ft_arg_type_words_len(char *s, char c)
{
	int		i;

	i = 0;
	if (s[i] && s[i] != c && !is_special_char(s[i]))
	{
		while (s[i] && s[i] != c && !is_special_char(s[i]))
		{
			if (s[i] == '\'' || s[i] == '"')
				i += arg_skip_quote(&s[i]);
			else if (s[i] == c || is_special_char(s[i]))
				break ;
			else if (s[i] && s[i] != c && !is_special_char(s[i]))
				i++;
		}
	}
	else if (is_special_char(s[i]))
		i += arg_special_char(&s[i]);
	return (i);
}

int	*ft_split_arg_type(char *s, char c)
{
	int		*tab;
	size_t	words;
	size_t	word_len;
	size_t	i;

	i = -1;
	words = ft_count_words(s, c);
	tab = (int *)calloc(sizeof(int), words);
	if (!tab || words == 0 || !s)
		return (NULL);
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
		if (s[0] == '"' || s[0] == '\'')
			tab[i] = 0;
		else if (s[0] == '>' || s[0] == '<')
			tab[i] = 1;
		word_len = ft_arg_type_words_len(s, c);
		s += word_len;
	}
	return (tab);
}
