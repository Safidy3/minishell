/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_valid_commad.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:47:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/17 16:03:28 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_symbol(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	symbols_condition(char *command, int i)
{
	if (command[i] == '<' || command[i] == '>' || command[i] == '|'
		|| command[i] == '\0')
		return (-1);
	return (0);
}

int ft_check_quote(char *command , char cote, int *i)
{
	if (command[*i] == cote)
	{
		(*i)++;
		while (command[*i] && command[*i] != cote)
			(*i)++;
		if (command[*i] == '\0')
			return(ft_putstr_fd("syntax error; unclosed quote\n", 2), -1);
	}
	return(1);
}

int	check_symbol(char *command, char c, int *i)
{
	*i = *i + 1;
	if(command[*i] == '\0')
		return (-1);
	if (ft_isspace(command[*i]))
	{
		while (ft_isspace(command[*i]))
			*i = *i + 1;
		if (symbols_condition(command, *i) == -1)
			return (-1);
	}
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

int	handle_pipe(int *flag_redirection, char *command, int *i)
{
	if (command[*i] == '|')
	{
		if (ft_isspace(command[*i]))
		{
			while (ft_isspace(command[*i]))
				i++;
			if (command[*i] == '|')
				*flag_redirection = 1;
		}
		else if (command[*i + 1] == '|' || command[*i + 1] == '\0')
			*flag_redirection = 1;
	}
	return (0);
}

int	handle_redirection(int *flag_redirection, char *command, int *i)
{
	if (command[*i] == '>' || command[*i] == '<')
	{
		if (check_symbol(command, command[*i], &(*i)) == -1)
			*flag_redirection = 1;
		if (!command[*i])
			return (1);
	}
	return (0);
}

int	print_syntax_error(int *flag_redirection)
{
	if (*flag_redirection == 1)
	{
		ft_putstr_fd("bash: syntax error\n", 1);
		return (0);
	}
	return (1);
}

int check_unlosed_quotes(char *command, int i)
{
    char quote;

    while (command[i] && command[i] != '\'' && command[i] != '\"')
        i++;
    if (command[i] == '\0')
        return 1;
    quote = command[i++];
    while (command[i] && command[i] != quote)
        i++;
    if (command[i] == '\0')
        return (ft_putstr_fd("unclosed quote\n", 2), 0);
    if (command[i] == quote && command[i + 1])
        return check_unlosed_quotes(command, i + 1);
    return (1);
}

int	is_valid_command(char *command)
{
	int	i;
	int	flag_redirection;

	flag_redirection = 0;
	i = 0;
	while (command[i] && ft_isspace(command[i]))
		i++;
	if (command[i] == '|')
	{
		ft_putstr_fd("bash: syntax error\n", 1);
		return (1);
	}
	if (command[i] == '\0')
		return (0);
	while (command[i])
	{
		if (ft_isspace(command[i]))
		{
			i++;
			continue ;
		}
		if (ft_check_quote(command, 34, &i) == -1 || ft_check_quote(command,39 ,&i) == -1)
			return(0);
		if (handle_redirection(&flag_redirection, command, &i) == 1)
			return(ft_putstr_fd("bash: syntax error\n", 1), 0);
		handle_pipe(&flag_redirection, command, &i);
		i++;
	}
	if (print_syntax_error(&flag_redirection) == 0)
		return (0);
	return (1);
}
