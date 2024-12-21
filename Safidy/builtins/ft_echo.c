/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 15:25:07 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_echo_flag(char *flag)
{
	if (*flag != '-')
		return (0);
	flag++;
	if (*flag != 'n')
		return (0);
	while (*flag)
	{
		if (*flag != 'n')
			return (0);
		flag++;
	}
	return (1);
}

int	ft_echo(char **commands)
{
	int	token_count;
	int	skip_newline;
	int	i;

	token_count = array_len(commands);
	skip_newline = 0;
	i = 0;
	if (token_count == 1)
		return (ft_putstr_fd("\n", 1), 0);
	while (commands[++i] && is_echo_flag(commands[i]))
		skip_newline = 1;
	if (!commands[i])
		return (0);
	while (commands[i] && i < token_count)
	{
		ft_putstr_fd(commands[i], 1);
		if (i < token_count - 1)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!skip_newline)
		ft_putstr_fd("\n", 1);
	return (0);
}
