#include "minishell.h"

// ls -la | grep mini | awk '{print $9}' | head -n 1
/*
color in C
	printf("This is red text\033[0m\n");
	30 – Black
	31 – Red
	32 – Green
	33 – Yellow
	34 – Blue
	35 – Magenta
	36 – Cyan
	37 – White
*/

/******************* split iteration ******************/

void	ft_free(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void	free_split_1(void *s)
{
	if ((char *) s)
		free((char *) s);
}

void	print_s(void *s)
{
	printf("%s \n", (char *) s);
}

void	split_iterate(void **array, void (*f)(void *))
{
	int	i;

	i = -1;
	while (array[++i])
		f(array[i]);
}

void	free_split(char **array)
{
	split_iterate((void **) array, free_split_1);
	free(array);
}

int	array_len(char **array)
{
	int	i;

	if (!array || !array[0])
		return (-1);
	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	print_split(char **array)
{
	split_iterate((void **) array, print_s);
}

/******************* list iteration ******************/

void	print_list(void *s)
{
	split_iterate((void *) s, print_s);
	printf("\n");
}

void	free_list_content(void *s)
{
	if (!s)
		return ;
	split_iterate(s, free_split_1);
	free(s);
}

void	free_list(t_list *s)
{
	t_list	*temp;

	ft_lstiter(s, free_list_content);
	while (s)
	{
		temp = s->next;
		free(s);
		s = temp;
	}
	free(s);
}

/******************* init list ******************/

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
		new_list = ft_lstnew((void *) ft_split_esc_2(arr_commands[i], ' '));
		if (!new_list)
			list_init_error(*commands_list, arr_commands);
		if (i == 0)
			*commands_list = new_list;
		else
			ft_lstadd_back(commands_list, new_list);
		free(arr_commands[i]);
	}
	free(arr_commands);
}

/******************* Exec && Pipe ******************/

void	exec_error(char *bin_path, t_list *commands_list, char *msg)
{
	ft_free(bin_path);
	free_list(commands_list);
	ft_putstr_fd("Exec err: faild to ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

char	*join_bin_path(t_list *commands_list, char *bin_path)
{
	char	*temp;
	char	*res;

	temp = ft_strjoin(bin_path, "/");
	ft_free(bin_path);
	if (!temp)
		return (NULL);
	res = ft_strjoin(temp,
			(char *)((char **)commands_list->content)[0]);
	free(temp);
	if (!res)
		return (NULL);
	return (res);
}

char	*get_bin_path(t_list *commands_list)
{
	char	**bin_paths;
	char	*bin_path;
	char	*result;
	int		i;

	result = NULL;
	bin_paths = ft_split(getenv("PATH"), ':');
	if (!bin_paths)
		return (NULL);
	i = array_len(bin_paths);
	while (--i >= 0)
	{
		bin_path = join_bin_path(commands_list, bin_paths[i]);
		if (bin_path && access(bin_path, F_OK | X_OK) == 0)
			result = bin_path;
		else
			ft_free(bin_path);
	}
	free(bin_paths);
	return (result);
}

void	dup_in(int fd[2], t_list *commands_list, char *bin_path, int closeall)
{
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exec_error(bin_path, commands_list, "dup2 pipe\n");
	close(fd[0]);
	if (closeall != 0)
		close(fd[1]);
}

void	dup_out(int fd[2], t_list *commands_list, char *bin_path, int closeall)
{
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		exec_error(bin_path, commands_list, "dup2 pipe\n");
	close(fd[1]);
	if (closeall != 0)
		close(fd[0]);
}

/*
	fd[2][2]
	fd[0] : prev
	fd[1] : current
*/

void	exec_child(t_list *command, int prev_fd[2], int current_fd[2], char **env)
{
	char *bin_path;

	bin_path = get_bin_path(command);
	if (!bin_path)
		exec_error(NULL, command, "get_bin_path failed\n");
	if (prev_fd[0] != -1)
		dup_in(prev_fd, command, bin_path, 0);
	if (command->next)
		dup_out(current_fd, command, bin_path, 1);
	execve(bin_path, (char **)command->content, env);
	exec_error(bin_path, command, "execve failed\n");
	free(bin_path);
}

t_list	*exec_parent(t_list *command, int prev_fd[2], int current_fd[2])
{
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command->next)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
	command = command->next;
	return (command);
}

void exec_commands(t_all *all)
{
	t_list	*command;
	pid_t	pid;
	int		prev_fd[2];
	int		current_fd[2];

	prev_fd[0] = -1;
	prev_fd[1] = -1;
	command = all->command_list;
	while (command)
	{
		if (command->next && pipe(current_fd) == -1)
			exec_error(NULL, command, "pipe creation failed\n");
		pid = fork();
		if (pid == 0)
			exec_child(command, prev_fd, current_fd, all->env);
		else
			command = exec_parent(command, prev_fd, current_fd);
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	while (wait(NULL) > 0);
}

/******************* main ******************/

	// env
	// cat<minishell.c<otherfile.txt : otherfile iany ni cateny
	// echo hello >minishell.c>otherfile.txt :  creer daoly fa le farany iiany no nisy hello
	// echo "$USER{alphaNum + _}$HOME" $?
	// cat << (herdoc)
	// shellevel

	// e"c"h"o" "hello world"
	// ls -la '|' grep Okt
	// grep "Okt" | awk '{print | $g}'

int	main(int argc, char **argv, char **env)
{
	char	**commands;
	char	*example_com;
	t_all	*all;
	t_list	*commands_list;

	(void)argc;
	(void)argv;

	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (0);
	all->env = env;
	all->command_list = NULL;
	commands_list = NULL;
	example_com = "ls -la | grep \"Oct\" | awk '{print $9}' | head -n 10 | grep 'm'i'n'i's'h'e'll.";
	printf("%s\n\n", example_com);

	commands = ft_split_esc(example_com, '|');
	print_split(commands);
	printf("\n\n");

	init_list(&commands_list, commands);
	ft_lstiter(commands_list, print_list);
	all->command_list = commands_list;

	printf("output :\n");
	exec_commands(all);

	free_list(commands_list);
	free(all);
	return (0);
}
