/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:29:02 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/06 14:15:55 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_all *all, char **command)
{
	int exit_status;
	(void) command;
	int i;
	int not_digit;

	i = 0;
	not_digit = 0;
	exit_status = all->exit_status;
	if (array_len(command) != 1)
	{
		if (command[1][i] == '-' || command[1][i] == '+')
			i++;
		while (command[1][i])
		{
			if (!ft_isdigit(command[1][i]))
			{
				ft_putstr_fd(" numeric argument required\n" , 2);
				not_digit = 1;
				break;
			}
			i++;
		}
		if (not_digit)
			exit_status = 2;
		else
			exit_status = ft_atoi(command[1]);
		if (array_len(command) > 2)
		{
			ft_putstr_fd(" too many arguments\n", 2);
			exit_status = 1;
		}
	}
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	free(command);
	free(all);
	// write(2,"exit\n",5);
	exit(exit_status);
	return (0);
}
