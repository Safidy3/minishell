/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/06 14:19:03 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_echo_flag(char *flag)
{
	if (*flag != '-')
		return (0);
	flag++;
	while (*flag)
	{
		if (*flag != 'n')
			return (0);
		flag++;
	}
	return (1);
}

int	ft_echo(char **tokens)
{
	int	token_count;
	int	start_index;
	int	skip_newline;
	int	i;

	token_count = array_len(tokens);
	start_index = 0;
	skip_newline = 0;
	i = 0;
	if (token_count > 0 && strcmp(tokens[0], "echo") == 0)
	{
		if (token_count > 1)
		{
			while (is_echo_flag(tokens[++i]))
			{
				start_index++;
				skip_newline = 1;
			}
		}
		while (++start_index < token_count)
		{
			ft_putstr_fd(tokens[start_index], 1);
			if (start_index < token_count - 1)
				ft_putstr_fd(" ", 1);
		}
		if (!skip_newline)
			ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd("Command not recognized or invalid input.\n", 1);
	return (0);
}
