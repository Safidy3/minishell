/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 17:04:11 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_program(t_all *all, char **command, int print_exit)
{
	int	exit_status;

	exit_status = all->exit_status;
	if (print_exit == 1)
		ft_putstr_fd("exit\n", 1);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	free(command);
	free(all);
	exit(exit_status);
}

int	check_valid_arg(char **command)
{
	int		i;
	char	*cmd_trimed;

	i = 0;
	if (command[1][0] == '\0')
		return (0);
	cmd_trimed = ft_strtrim(command[1], " ");
	if (cmd_trimed[i] == '-' || cmd_trimed[i] == '+')
		i++;
	while (cmd_trimed[i])
	{
		if (!ft_isdigit(cmd_trimed[i]))
		{
			free(cmd_trimed);
			return (0);
		}
		i++;
	}
	free(cmd_trimed);
	return (1);
}

long long	ft_exit(t_all *all, char **command)
{
	int	is_valid_number;

	if (array_len(command) == 1)
		exit_program(all, command, 1);
	is_valid_number = check_valid_arg(command);
	if (!is_valid_number)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(command[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		all->exit_status = 2;
		exit_program(all, command, 0);
	}
	if (array_len(command) > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	all->exit_status = ft_atoi_long_long(command[1]) % 256;
	exit_program(all, command, 1);
	return (0);
}
