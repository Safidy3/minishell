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

void	print_split(void *s)
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

/******************* list iteration ******************/

void	print_list(void *s)
{
	split_iterate((void *) s, print_split);
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
		new_list = ft_lstnew((void *) ft_split_esc(arr_commands[i], ' '));
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

/******************* Exec ******************/

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

void exec_commands(t_list *commands_list, char **env)
{
    char *bin_path;
    char *bin_path_2;
    int status;
    int fd[2];

    if (pipe(fd) == -1)
        exec_error(NULL, commands_list, "pipe creation failed\n");

    bin_path = get_bin_path(commands_list);
    if (!bin_path)
        exec_error(bin_path, commands_list, "execute command\n");
    
    int pid = fork();
    if (pid == -1)
        exec_error(bin_path, commands_list, "fork new process\n");
    else if (pid == 0)
    {
        // First child (writer)
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            exec_error(bin_path, commands_list, "dup2 failed\n");
        close(fd[1]);
        execve(bin_path, (char **) commands_list->content, env);
        exec_error(bin_path, commands_list, "execute command\n");
    }

    bin_path_2 = get_bin_path(commands_list->next);
    if (!bin_path_2)
        exec_error(bin_path, commands_list, "execute command\n");

    int pid2 = fork();
    if (pid2 == -1)
        exec_error(bin_path, commands_list, "fork new process\n");
    else if (pid2 == 0)
    {
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO) == -1)
            exec_error(bin_path_2, commands_list, "dup2 failed\n");
        close(fd[0]);
        execve(bin_path_2, (char **) commands_list->next->content, env);
        exec_error(bin_path_2, commands_list, "execute command\n");
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid, &status, 0);
    waitpid(pid2, &status, 0);
    free(bin_path);
    free(bin_path_2);
}

/******************* main ******************/

int	main(int argc, char **argv, char **env)
{
	char	*example_com;
	char	**commands;
	t_list	*commands_list;

	(void)argc;
	(void)argv;
	(void)env;
	commands_list = NULL;
	example_com = "ls -la | grep \"Okt\" | awk '{print $9}' | head -n 5 | grep 'minishel.c";
	commands = ft_split(example_com, '|');
	printf("%s\n\n", example_com);
	init_list(&commands_list, commands);
	ft_lstiter(commands_list, print_list);
	exec_commands(commands_list, env);
	free_list(commands_list);
}
