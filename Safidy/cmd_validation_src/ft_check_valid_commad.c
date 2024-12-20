/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_valid_commad.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 11:47:46 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 13:50:39 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe(int *flag_redirection, const char *command, int *i)
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

int	handle_redirection(int *flag_redirection, const char *command, int *i)
{
	if (command[*i] == '>' || command[*i] == '<')
	{
		if (check_symbol(command, command[*i], &(*i)) == -1)
			*flag_redirection = 1;
		if (!command[*i])
		{
			ft_putstr_fd("bash: syntax error near \
				unexpected token `newline'\n", 2);
			return (1);
		}
	}
	return (0);
}

int	is_valid_command(const char *command)
{
	int	i;
	int	flag_redirection;

	flag_redirection = 0;
	i = 0;
	while (command[i] && ft_isspace(command[i]))
		i++;
	if (command[i] == '|')
	{
		if (command[i + 1] == '|')
			return (ft_putstr_fd("bash: syntax error \
				near unexpected token `||'\n", 2), 0);
		return (ft_putstr_fd("bash: syntax error \
			near unexpected token `|'\n", 2), 0);
	}
	if (command[i] == '\0')
		return (0);
	if (error_checker_loop(command, i, &flag_redirection) == 0)
		return (0);
	if (print_syntax_error(&flag_redirection) == 0)
		return (0);
	return (1);
}

int	valid_command(const char *command, t_all *all)
{
	if (ft_strlen((char *)command) == 0)
		return (0);
	if (!is_valid_command(command))
	{
		all->exit_status = 2;
		return (0);
	}
	return (1);
}
