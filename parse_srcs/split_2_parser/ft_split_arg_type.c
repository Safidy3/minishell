/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_arg_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:28:24 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 16:28:44 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	arg_skip_quote(char *s)
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

int	arg_special_char(char *s)
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

size_t	ft_arg_type_words_len(char *s, char c)
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
	words = ft_count_words_2(s, c);
	tab = (int *)ft_calloc(sizeof(int), words);
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
