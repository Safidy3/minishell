#include "minishell.h"

#define MAX_VAR_LEN 256

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

static size_t	get_env_len(const char *s)
{
	size_t	total_len;
	char	var_name[MAX_VAR_LEN];
	size_t	i;
	char	*var_value;

	total_len = 0;
	while (*s)
	{
		if (*s == '$')
		{
			s++;
			i = 0;
			while (*s && (ft_isalnum(*s) || *s == '_') && i < MAX_VAR_LEN - 1)
				var_name[i++] = *s++;
			var_name[i] = '\0';
			var_value = getenv(var_name);
			if (var_value)
				total_len += ft_strlen(var_value);
			s--;
		}
		else
			total_len++;
		s++;
	}
	return (total_len);
}

static void	copy_env_var(const char **s, char **dst)
{
	char	var_name[MAX_VAR_LEN];
	char	*var_value;
	size_t	i;

	(*s)++;
	i = 0;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < 255)
		var_name[i++] = *(*s)++;
	var_name[i] = '\0';
	var_value = getenv(var_name);
	if (var_value)
	{
		ft_strlcpy(*dst, var_value, ft_strlen(var_value) + 1);
		*dst += ft_strlen(var_value);
	}
	(*s)--;
}

char	*replace_env_vars(const char *s)
{
	char	*result;
	char	*dst;
	int		in_quote;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char) * (get_env_len(s) + 1));
	if (!result)
		return (NULL);
	dst = result;
	in_quote = 0;
	while (*s)
	{
		if (*s == '\'')
			in_quote = !in_quote;
		if (*s == '$' && !in_quote)
			copy_env_var(&s, &dst);
		else
			*dst++ = *s;
		s++;
	}
	*dst = '\0';
	return (result);
}

/******************* Exec && Pipe ******************/

void	fd_error(char *bin_path, t_all *all)
{
	perror("fd error ");
	ft_free(bin_path);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	exit(EXIT_FAILURE);
}

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
	if (!res)
		return (NULL);
	return (free(temp), res);
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
	return (free(bin_paths), result);
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

char	*get_redirection_file_name(char *command)
{
	char	*redirection_file;
	int		i;

	i = 0;
	while (command[i] && (command[i] == '>' || command[i] == '<'
			|| ft_isspace(command[i])))
		i++;
	redirection_file = ft_strdup(&command[i]);
	if (!redirection_file)
		return (NULL);
	return (redirection_file);
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
			redirection_files[++j] = get_redirection_file_name(command[i]);
			if (!redirection_files[j])
				exec_error(NULL, all, "get_redrection_array\n");
		}
	}
	return (redirection_files);
}

char	**get_infile_array(char **command, t_all *all)
{
	char	**redirection_files;
	int		count;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	while (command[++i])
		if (command[i][0] == '<')
			count++;
	redirection_files = (char **)calloc(sizeof(char *), count + 1);
	i = -1;
	while (command[++i])
	{
		if (command[i][0] == '<')
		{
			redirection_files[++j] = get_redirection_file_name(command[i]);
			if (!redirection_files[j])
				exec_error(NULL, all, "get_redrection_array\n");
		}
	}
	return (redirection_files);
}

char	**get_outfile_array(char **command, t_all *all)
{
	char	**redirection_files;
	int		count;
	int		i;
	int		j;

	count = 0;
	i = -1;
	j = -1;
	while (command[++i])
		if (command[i][0] == '>')
			count++;
	redirection_files = (char **)calloc(sizeof(char *), count + 1);
	i = -1;
	while (command[++i])
	{
		if (command[i][0] == '>')
		{
			redirection_files[++j] = get_redirection_file_name(command[i]);
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

void	manage_in_out_file(char **in_files, char **out_files, t_all *all)
{
	int		fd;
	int		i;

	i = -1;
	while (in_files[++i])
	{
		fd = open(in_files[i], O_RDONLY);
		if (fd == -1)
			fd_error(NULL, all);
		if (dup2(fd, STDIN_FILENO) == -1)
			exec_error(NULL, all, "dup2 failed\n");
		close(fd);
	}
	i = -1;
	while (out_files[++i])
	{
		fd = open(out_files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			fd_error(NULL, all);
		if (dup2(fd, STDOUT_FILENO) == -1)
			exec_error(NULL, all, "open failed\n");
		close(fd);
	}
}


int	check_builtins(char *command, t_all *all)
{
	int	exit_status;

	exit_status = all->exit_status;
	if (!ft_strncmp(command, "export", ft_strlen("export")))
	{

		exec_error(NULL, all, "builtings\n");
	}
	else if (!ft_strncmp(command, "env", ft_strlen("env")))
	{
		ft_print_env(all->env_list);
		exec_error(NULL, all, "builtings\n");
	}
	else if (!ft_strncmp(command, "echo", ft_strlen("echo")))
	{

		exec_error(NULL, all, "builtings\n");
	}
	else if (!ft_strncmp(command, "cd", ft_strlen("cd")))
	{

		exec_error(NULL, all, "builtings\n");
	}
	else if (!ft_strncmp(command, "pwd", ft_strlen("pwd")))
	{

		exec_error(NULL, all, "builtings\n");
	}
	else if (!ft_strncmp(command, "exit", ft_strlen("exit")))
	{
		free_list(all->command_list);
		free_split(all->env_arr);
		ft_free_env_list(all->env_list);
		free(all);
		exit(exit_status);
	}
	return (0);
}

void	exec_child(t_list *command_arr, int prev_fd[2],
			int current_fd[2], t_all *all)
{
	char	*bin_path;
	char	**in_files;
	char	**out_files;
	char	**command;

	if (check_spetial_char((char **)command_arr->content) == 0)
		command = (char **)command_arr->content;
	else
	{
		command = get_new_command((char **)command_arr->content, all);
		in_files = get_infile_array((char **)command_arr->content, all);
		out_files = get_outfile_array((char **)command_arr->content, all);
		manage_in_out_file(in_files, out_files, all);
	}
	if (check_builtins(command[0], all))
		return ;
	bin_path = get_bin_path(command[0]);
	if (!bin_path)
		exec_error(NULL, all, "get_bin_path failed\n");
	if (prev_fd[0] != -1)
		dup_in(prev_fd, all, bin_path, 0);
	if (command_arr->next)
		dup_out(current_fd, all, bin_path, 1);
	printf("output :\n");
	execve(bin_path, command, all->env_arr);
	exec_error(bin_path, all, "execve failed\n");
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

	// export list=ls ; $list
	// export WWW=$(echo "hello world"); WWW=hello world
	
	// shellevel
	// cat << (heredoc)
	// signal
	// loop and readline

	// echo "$USER{alphaNum + _}$HOME"
	// cat<minishell.c<Makefile : Makefile iany ni cateny
	// <minishell.c cat<Makefile : Makefile iany ni cateny
	// echo hello >minishell.c>Makefile : creer daoly fa le farany iany no nisy hello
	// env
	// echo "$USER{alphaNum + _}$HOME"
	// echo '$HOME'
	// "$USER$HOME" : safandri/home/safandri
	// "$USER*9$HOME" : safandri*9/home/safandri
	// "$USERad14$HOME" : /home/safandri
	// e"c"h"o" "hello world"
	// ls -la '|' grep Okt
	// grep "Okt" | awk '{print | $g}'

// int main()
// {
// 	char *line = "$HOME";
// 	char *var_line = "$HOME";
// 	printf("before : %s HELLO WORLD\n", line);
// 	var_line = replace_env_vars(line);
// 	printf("after : %s HELLO WORLD\n", var_line);
// char *export_var[2] = {"BBB=$(echo \"hello world\")", NULL};
// ft_export(env_list, export_var);
// ft_print_export(env_list);
// }


int	main(int argc, char **argv, char **envp)
{
	char		**commands;
	char		*line;
	t_all		*all;
	t_list		*commands_list;
	t_env_list	*env_list;
	char		**env_arr;

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

    using_history();
    while (1)
	{
        line = readline(">: ");
        if (*line)
            add_history(line);
		line = replace_env_vars(line);
		commands = ft_split_esc(line, '|');
		ft_free(line);
		init_list(&commands_list, commands);
		all->command_list = commands_list;

		exec_commands(all);
		free_list(commands_list);
	}
	return (free(all), ft_free_env_list(env_list), free_split(env_arr), 0);
}





// int main() {
//     char *input;

//     // Initialize readline and history
//     using_history();

//     while (1)
// 	{
//         // Prompt for user input
//         input = readline("Enter a command: ");

//         if (input == NULL)
//             break;  // Exit if no input (Ctrl+D)

//         // If input is not empty, add it to history
//         if (*input)
//             add_history(input);

//         // Print the input to the console
//         printf("You entered: %s\n", input);

//         // Free the memory allocated by readline
//         free(input);
//     }

//     return 0;
// }