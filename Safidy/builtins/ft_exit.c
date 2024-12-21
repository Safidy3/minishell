/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 14:27:16 by safandri         ###   ########.fr       */
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
void	pint_error_exit(char **command, t_all *all)
{
	ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(command[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit_program(all, command, 0);
	all->exit_status = 2;
}

int	ft_exit(t_all *all, char **command)
{
	int	is_valid_number;

	if (array_len(command) == 1)
		exit_program(all, command, 1);
	is_valid_number = check_valid_arg(command);
	if (!is_valid_number)
		pint_error_exit(command, all);
	if (array_len(command) > 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	if (command[1][0] == '+'
		&& ft_strncmp(command[1], "+9223372036854775807", ft_strlen(command[1])) > 0)
		pint_error_exit(command, all);
	if (ft_strncmp(command[1], "9223372036854775807", ft_strlen(command[1])) > 0
		|| (ft_strlen(command[1]) > 19 && command[1][0] != '+'))
		pint_error_exit(command, all);
	all->exit_status = ft_atoi(command[1]) % 256;
	exit_program(all, command, 1);
	return (0);
}
