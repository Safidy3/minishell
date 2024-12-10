/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/10 15:20:09 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_all_struct(t_all *all, char **envp)
{
	all->exit_status = 0;
	all->command_list = NULL;
	all->env_list = NULL;
	int_lst_env(&all->env_list, envp);
	all->env_arr = list_to_array(all->env_list);
	all->fd_og[0] = dup(STDIN_FILENO);
	all->fd_og[1] = dup(STDOUT_FILENO);
	all->prev_fd[0] = -1;
	all->prev_fd[1] = -1;
}

void	pars_command(char *line, t_all *all)
{
	char	**commands;

	line = replace_env_vars(line, all);
	if (valid_command(line, all))
	{
		commands = ft_split_esc(line, '|');
		init_list(&all->command_list, commands);
	}
	ft_free(line);
}

void	proccess_command(t_all *all)
{
	char	*line;

	while (1)
	{
		put_signal_handlig(2);
		line = readline(">: ");
		if (flag == SIGINT)
		{
			flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			continue ;
		}
		if (!line)
			exit(all->exit_status);
		if (*line)
			add_history(line);
		line = replace_env_vars(line, all);
		pars_command(line, all);
		if (exec_commands(all) == -1)
		{
			free_list(all->command_list);
			continue ;
		}
		free_list(all->command_list);
	}
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_all	*all;

// 	(void)argc;
// 	(void)argv;
// 	all = (t_all *)malloc(sizeof(t_all));
// 	if (!all)
// 		return (0);
// 	init_all_struct(all, envp);
// 	proccess_command(all);
// 	return (0);
// }



int main(int argc, char **argv, char **envp)
{
	char **commands;
	char *line;
	t_all *all;

	(void)argc;
	(void)argv;
	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (0);
	init_all_struct(all, envp);

	while (1)
	{
		put_signal_handlig(2);
		if (flag == SIGINT)
		{
			printf("\n");
			flag = 0;
		}
		line = readline(">: ");
		if (flag == SIGINT)
		{
			flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			continue;
		}
		if (!line)
			exit(all->exit_status);
		if (*line)
			add_history(line);
		line = replace_env_vars(line, all);
		if (valid_command(line, all))
		{
			commands = ft_split_esc(line, '|');
			ft_free(line);
			init_list(&all->command_list, commands);
			if (exec_commands(all) == -1)
			{
				free_list(all->command_list);
				continue;
			}
			free_list(all->command_list);
		}
	}
	return (0);
}
