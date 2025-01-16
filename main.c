/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/21 15:21:16 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_def_main_handler(t_all *all)
{
	int	status;

	status = all->exit_status;
	printf("exit\n");
	close(all->fd_og[0]);
	close(all->fd_og[1]);
	ft_free_env_list(all->env_list);
	free_split(all->env_arr);
	free(all);
	exit(status);
}

int	process_line(t_all *all, char *line)
{
	char	**commands;

	if (!line)
		exit(all->exit_status);
	if (*line)
	{
		add_history(line);
		append_to_history(line);
	}
	line = replace_env_vars(line, all);
	if (!valid_command(line, all))
		return (free(line), 0);
	commands = ft_split_esc(line, '|');
	ft_free(line);
	init_list(&all->command_list, commands);
	exec_commands(all);
	free_list(all->command_list);
	return (1);
}

void	begin_loop(t_all *all)
{
	char	*line;

	while (1)
	{
		put_signal_handlig(2);
		if (g_flag == SIGINT)
		{
			printf("\n");
			g_flag = 0;
		}
		line = readline(">: ");
		if (line == NULL)
			sig_def_main_handler(all);
		if (g_flag == SIGINT)
		{
			all->exit_status = 130;
			g_flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			continue ;
		}
		if (!process_line(all, line))
			continue ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_all	*all;

	(void)argc;
	(void)argv;
	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (0);
	load_history();
	init_all_struct(all, envp);
	begin_loop(all);
	return (0);
}
