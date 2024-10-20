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
		ft_free((char *) s);
}

void	print_s(void *s)
{
	printf("\"%s\", ", (char *) s);
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
	printf("{");
	split_iterate((void *) s, print_s);
	printf("}\n");
}

void	free_list_content(void *s)
{
	if (!s)
		return ;
	split_iterate(s, free_split_1);
	ft_free(s);
}

void	free_list(t_list *s)
{
	t_list	*temp;

	ft_lstiter(s, free_list_content);
	while (s)
	{
		temp = s->next;
		ft_free(s);
		s = temp;
	}
	ft_free(s);
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

/******************	env var ********************/

static void	expand_env_var(char **s, char **res_ptr)
{
	char	var_name[256];
	char	*var_value;
	int		var_len;

	(*s)++;
	var_len = 0;
	while (**s && (ft_isalnum(**s) || **s == '_'))
		var_name[var_len++] = *(*s)++;
	var_name[var_len] = '\0';
	var_value = getenv(var_name);
	if (var_value)
	{
		strcpy(*res_ptr, var_value);
		*res_ptr += ft_strlen(var_value);
	}
}

static char	*replace_env_vars(char *s)
{
	char	*result;
	char	*res_ptr;
	int		in_quote;

	in_quote = 0;
	result = calloc(strlen(s) + 1, sizeof(char));
	if (!result)
		return (NULL);
	res_ptr = result;
	while (*s)
	{
		if (*s == '\'')
		{
			in_quote = !in_quote;
			*res_ptr++ = *s++;
		}
		else if (*s == '$' && !in_quote)
			expand_env_var(&s, &res_ptr);
		else
			*res_ptr++ = *s++;
	}
	*res_ptr = '\0';
	return (result);
}

/******************* Exec && Pipe ******************/

void	exec_error(char *bin_path, t_all *all, char *msg)
{
	ft_free(bin_path);
	free_list(all->command_list);
	ft_free(all);
	ft_putstr_fd("Exec err: faild to ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

void	dup_in(int fd[2], t_all *all, char *bin_path, int closeall)
{
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exec_error(bin_path, all, "dup2 pipe\n");
	close(fd[0]);
	if (closeall != 0)
		close(fd[1]);
}

void	dup_out(int fd[2], t_all *all, char *bin_path, int closeall)
{
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		exec_error(bin_path, all, "dup2 pipe\n");
	close(fd[1]);
	if (closeall != 0)
		close(fd[0]);
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

int	check_spetial_char(char **command)
{
	int	i;

	i = -1;
	while (command[++i])
		if (command[i][0] == '>' || command[i][0] == '<' )
			return (1);
	return (0);
}

char	**get_redrection_array(char **command, t_all *all)
{
	char	*redirection_files[100];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (command[++i])
	{
		if (command[i][0] == '>' || command[i][0] == '<')
		{
			redirection_files[++j] = ft_strdup(command[i]);
			if (!redirection_files[j])
				exec_error(NULL, all, "get_redrection_array\n");
			printf("redirection_files[j] = %s\n", redirection_files[j]);
		}
	}
	return (redirection_files);
}

char	**get_new_command(char **command, t_all *all)
{
	char	*new_command[100];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (command[++i])
	{
		if (command[i][0] != '>' && command[i][0] != '<')
		{
			new_command[++j] = ft_strdup(command[i]);
			if (!new_command[j])
				exec_error(NULL, all, "get_redrection_array\n");
			printf("new_command[j] = %s\n", new_command[j]);
		}
	}
	return (new_command);
}

void	exec_child(t_list *command, int prev_fd[2],
			int current_fd[2], t_all *all)
{
	char	*bin_path;
	char	**redirection_files;
	char	**new_command;

	if (check_spetial_char((char **)command->content) == 0)
	{
		bin_path = get_bin_path(command);
		if (!bin_path)
			exec_error(NULL, all, "get_bin_path failed\n");
		if (prev_fd[0] != -1)
			dup_in(prev_fd, all, bin_path, 0);
		if (command->next)
			dup_out(current_fd, all, bin_path, 1);
		execve(bin_path, (char **)command->content, all->env);
		exec_error(bin_path, all, "execve failed\n");
		free(bin_path);
	}
	else
	{
		redirection_files = get_redrection_array((char **)command->content, all);
		new_command = get_new_command((char **)command->content, all);
		// printf("redirection file :\n");
		// print_split(redirection_files);
		exec_error(NULL, all, "get_bin_path failed\n");
	}
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

int	get_exit_stat(pid_t pids[MAX_COMMANDS], int command_count)
{
	int	status;
	int	i;

	i = 0;
	while (++i < command_count)
		waitpid(pids[i], &status, 0);
	return (WEXITSTATUS(status));
}

void	exec_commands(t_all *all)
{
	t_list	*command;
	pid_t	pids[MAX_COMMANDS];
	int		prev_fd[2];
	int		current_fd[2];
	int		command_count;

	command_count = 0;
	prev_fd[0] = -1;
	prev_fd[1] = -1;
	command = all->command_list;
	while (command)
	{
		if (command->next && pipe(current_fd) == -1)
			exec_error(NULL, command, "pipe creation failed\n");
		pids[command_count] = fork();
		if (pids[command_count] == 0)
			exec_child(command, prev_fd, current_fd, all);
		else if (pids[command_count++] > 0)
			command = exec_parent(command, prev_fd, current_fd);
		else
			exec_error(NULL, command, "fork failed\n");
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	all->exit_status = get_exit_stat(pids, command_count);
}

/******************* main ******************/

	// env
	// cat<minishell.c<Makefile : Makefile iany ni cateny
	// <minishell.c cat<Makefile : Makefile iany ni cateny
	// echo hello >minishell.c>Makefile : creer daoly fa le farany iany no nisy hello
	// shellevel
	// cat << (heredoc)

	// echo "$USER{alphaNum + _}$HOME"
	// echo '$HOME'
	// "$USER$HOME" : safandri/home/safandri
	// "$USER*9$HOME" : safandri*9/home/safandri
	// "$USERad14$HOME" : /home/safandri
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
	all->exit_status = 0;
	all->env = env;
	all->command_list = NULL;
	commands_list = NULL;
	// example_com = "ls -la | grep \"Okt\" | awk '{print $9}' | head -n 10 | grep 'm'i'n'i's'h'e'll.";
	example_com = "<   minishell.c cat<Makefile";
	printf("%s\n\n", example_com);

	example_com = replace_env_vars(example_com);
	commands = ft_split_esc(example_com, '|');
	ft_free(example_com);

	// printf("split command :\n");
	// print_split(commands);
	// printf("\n\n");

	init_list(&commands_list, commands);
	printf("list command :\n");
	ft_lstiter(commands_list, print_list);
	printf("\n\n");
	all->command_list = commands_list;

	printf("output :\n");
	exec_commands(all);
	printf("\n\n");

	printf("in parent\n");
	free_list(commands_list);
	free(all);
	return (0);
}

