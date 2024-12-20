/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validator_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:46:37 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 13:50:47 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_symbol(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	symbols_condition(const char *command, int i)
{
	if (command[i] == '<' || command[i] == '>' || command[i] == '|'
		|| command[i] == '\0')
		return (-1);
	return (0);
}

int	print_syntax_error(int *flag_redirection)
{
	if (*flag_redirection == 1)
		return (ft_putstr_fd("bash: syntax error\n", 1), 0);
	return (1);
}
