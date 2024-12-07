/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/07 14:32:49 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
	all->exit_status = 0;
	all->command_list = NULL;
	all->env_list = NULL;
	int_lst_env(&all->env_list, envp);
	all->env_arr = list_to_array(all->env_list);
	all->fd_og[0] = dup(STDIN_FILENO);
	all->fd_og[1] = dup(STDOUT_FILENO);

	while (1)
	{
		put_signal_handlig(2);
		// if (flag == SIGINT)
		// {
		// 	printf("\n");
		// 	flag = 0;
		// }
		line = readline(">: ");
		if (flag == SIGINT)
		{
			flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			continue;
		}
		if (!line)
		{
			write(2,"exit\n",5);
			exit(all->exit_status);
		}
		if (*line)
			add_history(line);
		line = replace_env_vars(line, all);

		commands = ft_split_esc(line, '|');
		if (valid_command(line, all))
		{
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




// int	main(int argc, char **argv, char **envp)
// {
// 	char	**commands;
// 	char	*line;
// 	t_all	*all;

// 	(void) argc;
// 	(void) argv;
// 	all = (t_all *)malloc(sizeof(t_all));
// 	if (!all)
// 		return (0);
// 	all->exit_status = 0;
// 	all->command_list = NULL;
// 	all->env_list = NULL;
// 	int_lst_env(&all->env_list, envp);
// 	all->env_arr = list_to_array(all->env_list);

// 	line = "echo $?HOME";
// 	line = replace_env_vars(line, all);

// 	commands = ft_split_esc(all, line, '|');
// 	if (valid_command(line, all))
// 	{
// 		ft_free(line);
// 		init_list(&all->command_list, commands);
// 		// printf("%d\n", ft_lstsize(all->command_list));
// 		exec_commands(all);
// 		// printf("exit stat = %d\n", all->exit_status);
// 		free_list(all->command_list);
// 	}
// 	return (0);
// }





/***********************  tester  **************************/

// int	ft_launch_minishell(char *line, char **envp)
// {
// 	char	**commands;
// 	t_all	*all;

// 	all = (t_all *)malloc(sizeof(t_all));
// 	if (!all)
// 		return (0);
// 	all->exit_status = 0;
// 	all->command_list = NULL;
// 	all->env_list = NULL;
// 	int_lst_env(&all->env_list, envp);
// 	all->env_arr = list_to_array(all->env_list);
// 	if (*line)
// 		add_history(line);
// 	line = replace_env_vars(line, all);
// 	commands = ft_split_esc(all, line, '|');
// 	if (valid_command(line, all))
// 	{
// 		ft_free(line);
// 		init_list(&all->command_list, commands);
// 		exec_commands(all);
// 		free_list(all->command_list);
// 	}
// 	return (all->exit_status);
// 	// return (free_all_struct(all),  all->exit_status);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	(void) argc;
// 	(void) argv;
// 	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
// 	{
// 		int exit_status = ft_launch_minishell(argv[2], envp);
// 		exit(exit_status);
// 	}
// }