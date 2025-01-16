/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:46:42 by safandri          #+#    #+#             */
/*   Updated: 2024/12/20 11:46:59 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_print_export_error(char *command)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd("'", 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

int	ft_export_error(char *variable_name, char *command)
{
	if (variable_name)
	{
		if (variable_name[0] == '\0' || (!ft_isalpha(variable_name[0])
				&& variable_name[0] != '_'))
		{
			ft_print_export_error(command);
			return (1);
		}
		else if (ft_check_after_first_caracter(variable_name, command) == 1)
			return (1);
	}
	return (0);
}

int	check_env_error(char *command, char *variable_name, int *return_value)
{
	if (!variable_name)
	{
		*return_value = 1;
		return (ft_print_export_error(command), 1);
	}
	if (ft_export_error(variable_name, command) == 1)
	{
		*return_value = 1;
		return (free(variable_name), 2);
	}
	return (0);
}
