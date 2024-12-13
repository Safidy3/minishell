/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/13 14:52:17 by larakoto         ###   ########.fr       */
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
	all->in_pipe[0] = -1;
	all->in_pipe[1] = -1;
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


// refa tsisy commande an aloha de mi-segfault,Filaharan le commande ex cat << a << b Makfile
// tkn cat Makfile

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
		{
			printf("exit\n");
			free_list(all->command_list);
			free_split(all->env_arr);
			ft_free_env_list(all->env_list);
			free(all);
			exit(0);
		}
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
			if (all->exit_status == 131)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			free_list(all->command_list);
		}
	}
	return (0);
}
