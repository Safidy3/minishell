/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/19 13:08:49 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_init_error(t_list *commands_list, char **arr_commands)
{
	split_iterate((void **) arr_commands, free_split_1);
	free(arr_commands);
	free_list(commands_list);
	printf("Allocation error \033[0m: faild to allocate memory\n");
	exit(EXIT_FAILURE);
}

void	init_list(t_list **commands_list, char **arr_commands)
{
	t_list	*new_list;
	int		i;

	i = -1;
	while (arr_commands[++i])
	{
		new_list = ft_newlst((void *) ft_split_esc_2(arr_commands[i], ' '),
				ft_split_arg_type(arr_commands[i], ' '));
		if (!new_list)
			list_init_error(*commands_list, arr_commands);
		if (i == 0)
			*commands_list = new_list;
		else
			ft_add_back_lst(commands_list, new_list);
		free(arr_commands[i]);
	}
	free(arr_commands);
}

void	init_all_struct(t_all *all, char **envp)
{
	all->exit_status = 0;
	all->command_list = NULL;
	all->env_list = NULL;
	int_lst_env(&all->env_list, envp);
	all->env_arr = list_to_array(all->env_list);
	all->fd_og[0] = dup(STDIN_FILENO);
	all->fd_og[1] = dup(STDOUT_FILENO);
	all->in_pipe[0] = -1;
	all->in_pipe[1] = -1;
}

void	sig_def_main_handler(t_all *all)
{
	int		status;

	status = all->exit_status;
	printf("\n");
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
		add_history(line);
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
	init_all_struct(all, envp);
	begin_loop(all);
	return (0);
}
