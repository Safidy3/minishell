/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_esc_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:14:43 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 16:28:59 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_special_char(char *s, int *word_len, int *i)
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

size_t	ft_count_words_len_2(char *s, char c)
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

size_t	ft_count_str_len(char *s, char c)
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
	return (i);
}

void	cpy_to_arr(char *s, char *tab, int word_len)
{
	char	quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (word_len == 0)
		return ;
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
}

char	**ft_split_esc_2(char *s, char c)
{
	char	**tab;
	size_t	words;
	size_t	word_len;
	size_t	i;

	i = -1;
	words = ft_count_words_2(s, c);
	tab = (char **)ft_calloc(sizeof(char *), (words + 1));
	if (!tab || words == 0 || !s)
		return (NULL);
	while (++i < words)
	{
		s = ft_escap_spliter(s, c);
		word_len = ft_count_words_len_2(s, c);
		tab[i] = (char *)ft_calloc(sizeof(char), (word_len + 1));
		if (!tab[i])
			return (ft_free_exit(tab));
		cpy_to_arr(s, tab[i], word_len);
		s += ft_count_str_len(s, c);
	}
	return (tab);
}
