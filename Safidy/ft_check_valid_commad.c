/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_valid_commad.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:47:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 12:16:02 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_symbol(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}



int	check_symbol(char *command, char c, int *i)
{
	*i = *i + 1;
	if (ft_isspace(command[*i]))
	{
		while (ft_isspace(command[*i]))
			*i = *i + 1;
		if (command[*i] == '<' || command[*i] == '>' || command[*i] == '|'
			|| command[*i] == '\0')
			return (-1);
	}
	if (command[*i] == c)
	{
		*i = *i + 1;
		if (command[*i] == '<' || command[*i] == '>' || command[*i] == '|'
			|| command[*i] == '\0')
			return (-1);
		if (ft_isspace(command[*i]))
		{
			while (ft_isspace(command[*i]))
				*i = *i + 1;
			if (command[*i] == '<' || command[*i] == '>' || command[*i] == '|'
				|| command[*i] == '\0')
				return (-1);
		}
	}
	if (is_symbol(command[*i]))
		return (-1);
	return (0);
}

int	is_valid_command(char *command)
{
	int	i;
	int	flag_redirection;

	flag_redirection = 0;
	i = 0;
	while (command[i] && ft_isspace(command[i]))
		i++;
	if (command[i] == '\0')
		return (0);
	while (command[i])
	{
		if (ft_isspace(command[i]))
		{
			i++;
			continue ;
		}
		if (command[i] == '>' || command[i] == '<')
		{
			if (check_symbol(command, command[i], &(i)) == -1)
				flag_redirection = 1;
		}
		if (command[i] == '|')
		{
			if (ft_isspace(command[i]))
			{
				while (ft_isspace(command[i]))
					i++;
				if (command[i] == '|')
					flag_redirection = 1;
			}
			else if (command[i + 1] == '|' || command[i + 1] == '\0')
				flag_redirection = 1;
		}
		i++;
	}
	if (flag_redirection == 1)
	{
		ft_putstr_fd("bash: syntax error\n", 1);
		return (0);
	}
	return (1);
}
