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
	printf("%s\n", (char *) s);
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
		new_list = ft_newlst((void *) ft_split_esc_2(arr_commands[i], ' '));
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
	result = (char *)calloc(ft_strlen(s) + 1, sizeof(char));
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
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	ft_putstr_fd("Exec err: faild to ", 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
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

char	*join_bin_path(char *commands_list, char *bin_path)
{
	char	*temp;
	char	*res;

	temp = ft_strjoin(bin_path, "/");
	ft_free(bin_path);
	if (!temp)
		return (NULL);
	res = ft_strjoin(temp, commands_list);
	free(temp);
	if (!res)
		return (NULL);
	return (res);
}

char	*get_bin_path(char *commands_list)
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
		{
            if (result)
                free(result);
			result = bin_path;
		}
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
	char	**redirection_files;
	int		count;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	while (command[++i])
		if (command[i][0] == '>' || command[i][0] == '<')
			count++;
	redirection_files = (char **)calloc(sizeof(char *), count + 1);
	i = -1;
	while (command[++i])
	{
		if (command[i][0] == '>' || command[i][0] == '<')
		{
			redirection_files[++j] = command[i];
			if (!redirection_files[j])
				exec_error(NULL, all, "get_redrection_array\n");
		}
	}
	return (redirection_files);
}

char	**get_new_command(char **command, t_all *all)
{
	char	**new_command;
	int		count;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	while (command[++i])
		if (command[i][0] != '>' && command[i][0] != '<')
			count++;
	new_command = (char **)calloc(sizeof(char *), count + 1);
	i = -1;
	while (command[++i])
	{
		if (command[i][0] != '>' && command[i][0] != '<')
		{
			new_command[++j] = command[i];
			if (!new_command[j])
				exec_error(NULL, all, "get_redrection_array\n");
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
		bin_path = get_bin_path((char *)((char **)command->content)[0]);
		if (!bin_path)
			exec_error(NULL, all, "get_bin_path failed\n");
		if (prev_fd[0] != -1)
			dup_in(prev_fd, all, bin_path, 0);
		if (command->next)
			dup_out(current_fd, all, bin_path, 1);
		execve(bin_path, (char **)command->content, all->env_arr);
		exec_error(bin_path, all, "execve failed\n");
	}
	else
	{
		redirection_files = get_redrection_array((char **)command->content, all);
		new_command = get_new_command((char **)command->content, all);

		printf("redirection file :\n");
		print_split(redirection_files);
		printf("\n\n");

		printf("New command :\n");
		print_split(new_command);
		printf("\n\n");

		bin_path = get_bin_path(new_command[0]);
		if (!bin_path)
			exec_error(NULL, all, "get_bin_path failed\n");
		printf("binpath = %s\n\n", bin_path);

		free(redirection_files);
		free(new_command);
		exec_error(bin_path, all, "nothing\n");
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
			exec_error(NULL, all, "pipe creation failed\n");
		pids[command_count] = fork();
		if (pids[command_count] == 0)
			exec_child(command, prev_fd, current_fd, all);
		else if (pids[command_count++] > 0)
			command = exec_parent(command, prev_fd, current_fd);
		else
			exec_error(NULL, all, "fork failed\n");
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	all->exit_status = get_exit_stat(pids, command_count);
}

/******************* main ******************/

	// signal
	// cat<minishell.c<Makefile : Makefile iany ni cateny
	// <minishell.c cat<Makefile : Makefile iany ni cateny
	// echo hello >minishell.c>Makefile : creer daoly fa le farany iany no nisy hello
	// shellevel
	// cat << (heredoc)

	// env
	// echo "$USER{alphaNum + _}$HOME"
	// echo '$HOME'
	// "$USER$HOME" : safandri/home/safandri
	// "$USER*9$HOME" : safandri*9/home/safandri
	// "$USERad14$HOME" : /home/safandri
	// e"c"h"o" "hello world"
	// ls -la '|' grep Okt
	// grep "Okt" | awk '{print | $g}'

int	main(int argc, char **argv, char **envp)
{
	char			**commands;
	char			*line;
	t_all			*all;
	t_list			*commands_list;
	t_env_list		*env_list;
	char			**env_arr;

	(void)argc;
	(void)argv;
	commands_list = NULL;
	env_list = NULL;
	int_lst_env(&env_list, envp);
	env_arr = list_to_array(env_list);

	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (0);
	all->exit_status = 0;
	all->env_arr = env_arr;
	all->env_list = env_list;
	all->command_list = NULL;

	// line = readline("minishel >");
	// if (!line[0])
	// 	printf("tafiditra\n");
	// line = "ls -la | grep \"Oct\" | awk '{print $9}' | head -n 10 | grep 'm'i'n'i's'h'e'll.";

	line = "<   minishell.c cat<Makefile";
	printf("%s\n\n", line);

	line = replace_env_vars(line);
	commands = ft_split_esc(line, '|');
	ft_free(line);

	// printf("split command :\n");
	// print_split(commands);
	// printf("\n\n");

	init_list(&commands_list, commands);
	all->command_list = commands_list;
	printf("list command :\n");
	ft_lstiter(commands_list, print_list);
	printf("\n\n");

	printf("output :\n");
	exec_commands(all);
	printf("\n\n");

	free_split(env_arr);
	ft_free_env_list(env_list);

	free_list(commands_list);
	free(all);
	return (0);
}




// int	main(int argc, char **argv, char **envp)
// {
// 	char			*line;
// 	t_env_list	*env_list;
// 	char			**env;
// 	char			**commande;

// 	env_list = NULL;
// 	int_lst_env(&env_list, envp);
// 	env = list_to_array(env_list);

// 	line = "export LS_COLORS";
// 	commande = ft_split_esc_2(line, ' ');
// 	print_split(commande);
// 	printf("\n\n\n\n");

// 	// ft_export(env_list, commande);
// 	// ft_print_env(env_list);
// 	printf("\n\n\n\n");
// 	ft_unset(&env_list, commande);
// 	ft_prin_export(env_list);

// 	free_split(commande);
// 	free_split(env);
// 	ft_free_env_list(&env_list);
// }
