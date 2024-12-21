/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_2_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:06:54 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 12:09:41 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_escape_special_char(char *s)
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

size_t	ft_count_words_2(char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s == '\0')
			break ;
		count++;
		while (*s && *s != c)
		{
			if (*s == '\'' || *s == '\"')
				s = ft_escape_quote(s);
			else if (is_special_char(*s))
			{
				count++;
				s = ft_escape_special_char(s);
			}
			else
				s++;
		}
	}
	return (count);
}

void	skip_quote(char *s, int *word_len, int *i)
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

int	is_special_char(char c)
{
	return (c == '<' || c == '>');
}

char	*ft_escap_spliter(char *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}
