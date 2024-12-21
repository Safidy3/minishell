/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_2_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:06:54 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 16:41:56 by safandri         ###   ########.fr       */
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
