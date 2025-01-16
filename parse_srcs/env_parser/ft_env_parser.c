/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:08:16 by safandri          #+#    #+#             */
/*   Updated: 2024/12/19 14:53:21 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*handle_quoted_delimiter(char **s, char *dst, char quote_type)
{
	char	opposite_quote;

	if (quote_type == '\'')
		opposite_quote = '"';
	else
		opposite_quote = '\'';
	dst = copy_char(dst, opposite_quote);
	dst = copy_char(dst, quote_type);
	(*s)++;
	while (**s && **s != quote_type)
		dst = copy_char(dst, *(*s)++);
	dst = copy_char(dst, *(*s)++);
	dst = copy_char(dst, opposite_quote);
	return (dst);
}

char	*handle_heredoc_delimiter(char **s, char *dst)
{
	dst = copy_char(dst, **s);
	(*s)++;
	dst = copy_char(dst, **s);
	(*s)++;
	while (isspace(**s))
		dst = copy_char(dst, *(*s)++);
	if (**s == '\'')
		dst = handle_quoted_delimiter(s, dst, '\'');
	else if (**s == '"')
		dst = handle_quoted_delimiter(s, dst, '"');
	else
		while (**s && !isspace(**s))
			dst = copy_char(dst, *(*s)++);
	return (dst);
}

void	append_quoted_text(char **dst, char **s, char quote, t_all *all)
{
	*dst = copy_char(*dst, *(*s)++);
	while (**s && **s != quote)
	{
		if (**s == '$' && quote == '"' )
			append_env_value_quote(dst, s, all);
		else if (**s == '\\' && *(*s + 1) == '$')
		{
			*dst = copy_char(*dst, *(*s + 1));
			*s += 2;
		}
		else
			*dst = copy_char(*dst, *(*s)++);
	}
	if (*s && **s == quote)
		*dst = copy_char(*dst, *(*s)++);
}

int	if_path(char *s)
{
	if (*s == '~' && (!*(s + 1) || ft_isspace(*(s + 1))
			|| *(s + 1) == '/') && (!*(s - 1) || ft_isspace(*(s - 1))))
		return (1);
	return (0);
}

char	*replace_env_vars(char *s, t_all *all)
{
	char	*dst;

	if (initiate_dst(&dst, s) == 0)
		return (s);
	while (*s)
	{
		if (*s == '\'')
			append_quoted_text(&dst, &s, '\'', all);
		else if (*s == '\"')
			append_quoted_text(&dst, &s, '\"', all);
		else if (*s == '\\' && *(s + 1) == '$')
		{
			dst = copy_char(dst, *(s + 1));
			s += 2;
		}
		else if (*s == '<' && *(s + 1) == '<')
			dst = handle_heredoc_delimiter(&s, dst);
		else if (*s == '$' && *(s + 1))
			append_env_value(&dst, &s, all);
		else if (if_path(s))
			dst = expand_path(&s, &dst, all);
		else
			dst = copy_char(dst, *s++);
	}
	return (dst);
}
