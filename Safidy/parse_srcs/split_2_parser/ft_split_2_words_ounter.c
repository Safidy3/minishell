/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_2_words_ounter.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:41:46 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 17:17:46 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*handl_words_counting(char *s, char c)
{
	char	quote;

	while (*s && *s != c)
	{
		if (is_special_char(*s))
			break ;
		if (*s == '\'' || *s == '\"')
		{
			quote = *s;
			s++;
			while (*s && *s != quote)
				s++;
			if (*s == quote)
				s++;
		}
		else
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
		if (is_special_char(*s))
		{
			count++;
			s = ft_escape_special_char(s);
			continue ;
		}
		count++;
		s = handl_words_counting(s, c);
	}
	return (count);
}
