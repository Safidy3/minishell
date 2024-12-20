/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larakoto < larakoto@student.42antananar    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 10:29:42 by larakoto          #+#    #+#             */
/*   Updated: 2024/12/20 17:32:12 by larakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_def_main_handler(t_all *all)
{
	int	status;

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
			all->exit_status = 130;
			g_flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			continue ;
		}
		if (!process_line(all, line))
			continue ;
	}
}
/*
cd ""
exit "  42  "
exit llong max
ctrl+ c in prompt => exit status tokony 130

>: export PATH=dirc1:$PATH
>: mkdir dirc1
>: touch dirc1/ls
>: chmod 777 dirc1/ls
>: ls
==520812==
==520812== FILE DESCRIPTORS: 3 open (3 std) at exit.
==520812==
==520812== HEAP SUMMARY:
==520812==     in use at exit: 208,467 bytes in 240 blocks
==520812==   total heap usage: 973 allocs, 733 frees, 316,369 bytes allocated
==520812==
==520812== 9 bytes in 1 blocks are definitely lost in loss record 5 of 69
==520812==    at 0x4848899: malloc (in
		/usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==520812==    by 0x4097D5: ft_strjoin (in
		/home/safandri/Documents/minishell/Safidy/minishell)
==520812==    by 0x4046A4: join_bin_path (ft_bin_path.c:24)
==520812==    by 0x40479A: find_bin (ft_bin_path.c:54)
==520812==    by 0x4048D2: get_bin_path (ft_bin_path.c:85)
==520812==    by 0x4050FE: get_command_bin (exec_geter.c:93)
==520812==    by 0x403E48: exec_child (ft_exec.c:71)
==520812==    by 0x403F62: exec_forked (ft_exec.c:93)
==520812==    by 0x4040C6: exec_commands (ft_exec.c:120)
==520812==    by 0x408A6C: process_line (main.c:43)
==520812==    by 0x408B33: begin_loop (main.c:69)
==520812==    by 0x408BA4: main (main.c:89)
==520812==
==520812== 16 bytes in 1 blocks are definitely lost in loss record 11 of 69
==520812==    at 0x4848899: malloc (in
		/usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==520812==    by 0x409495: ft_calloc (in
		/home/safandri/Documents/minishell/Safidy/minishell)
==520812==    by 0x404FA8: get_new_command (exec_geter.c:74)
==520812==    by 0x40505C: get_command_bin (exec_geter.c:86)
==520812==    by 0x403E48: exec_child (ft_exec.c:71)
==520812==    by 0x403F62: exec_forked (ft_exec.c:93)
==520812==    by 0x4040C6: exec_commands (ft_exec.c:120)
==520812==    by 0x408A6C: process_line (main.c:43)
==520812==    by 0x408B33: begin_loop (main.c:69)
==520812==    by 0x408BA4: main (main.c:89)
==520812==
==520812== LEAK SUMMARY:
==520812==    definitely lost: 25 bytes in 2 blocks
==520812==    indirectly lost: 0 bytes in 0 blocks
==520812==      possibly lost: 0 bytes in 0 blocks
==520812==    still reachable: 0 bytes in 0 blocks
==520812==         suppressed: 208,442 bytes in 238 blocks
==520812==
==520812== For lists of detected and suppressed errors, rerun with: -s
==520812== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 1 from 1)


*/
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
