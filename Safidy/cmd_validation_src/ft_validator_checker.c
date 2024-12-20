/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validator_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:48:16 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 13:49:37 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_quote(const char *command, char cote, int *i)
{
	if (command[*i] == cote)
	{
		(*i)++;
		while (command[*i] && command[*i] != cote)
			(*i)++;
		if (command[*i] == '\0')
		{
			ft_putstr_fd("syntax error; unclosed quote\n", 2);
			return (-1);
		}
	}
	return (1);
}

int	check_unlosed_quotes(char *command, int i)
{
	char	quote;

	while (command[i] && command[i] != '\'' && command[i] != '\"')
		i++;
	if (command[i] == '\0')
		return (1);
	quote = command[i++];
	while (command[i] && command[i] != quote)
		i++;
	if (command[i] == '\0')
		return (ft_putstr_fd("unclosed quote\n", 2), 0);
	if (command[i] == quote && command[i + 1])
		return (check_unlosed_quotes(command, i + 1));
	return (1);
}

int	check_symbol(const char *command, char c, int *i)
{
	*i = *i + 1;
	if (command[*i] == '\0')
		return (-1);
	if (ft_isspace(command[*i]) && check_space_and_symbols(command, i) == -1)
		return (-1);
	if (command[*i] == c)
	{
		*i = *i + 1;
		if (command[*i] == '<' || command[*i] == '>'
			|| command[*i] == '|' || command[*i] == '\0')
			return (-1);
		if (ft_isspace(command[*i]))
		{
			while (ft_isspace(command[*i]))
				*i = *i + 1;
			if (symbols_condition(command, *i) == -1)
				return (-1);
		}
	}
	if (is_symbol(command[*i]))
		return (-1);
	(*i)--;
	return (0);
}

int	error_checker_loop(const char *command, int i, int *flag_redirection)
{
	while (command[i])
	{
		if (ft_isspace(command[i]))
		{
			i++;
			continue ;
		}
		if (ft_check_quote(command, 34, &i) == -1
			|| ft_check_quote(command, 39, &i) == -1)
			return (0);
		if (handle_redirection(flag_redirection, command, &i) == 1)
			return (0);
		handle_pipe(flag_redirection, command, &i);
		i++;
	}
	return (1);
}

int	check_space_and_symbols(const char *command, int *i)
{
	while (ft_isspace(command[*i]))
		*i = *i + 1;
	if (symbols_condition(command, *i) == -1)
		return (-1);
	return (0);
}
