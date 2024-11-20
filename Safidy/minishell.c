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

void	print_redir(t_redirect **redirections)
{
	int	i;

	i = -1;
	printf("redirections :\n");
	while (redirections[++i])
		printf("%s %d, ", redirections[i]->filename, redirections[i]->type);
	printf("\n\n\n");
}

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

void	print_command_list(t_list *commands)
{
	while (commands)
	{
		split_iterate((void *) commands->content, print_s);
		commands = commands->next;
	}
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

void	free_all_struct(t_all *all)
{
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
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

char	*ft_getenv(char *env_var, t_all *all)
{
	t_env_list	*tmp;
	char		*res;

	tmp = all->env_list;
	res = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->first, env_var) == 0)
		{
			res = ft_strdup(tmp->second);
			break;
		}
		tmp = tmp->next;
	}
	return (res);
}

static size_t	get_env_len(const char *s, t_all *all)
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
			var_value = ft_getenv(var_name, all);
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

static void	copy_env_var(const char **s, char **dst, t_all *all)
{
	char	var_name[MAX_VAR_LEN];
	char	*var_value;
	size_t	i;

	(*s)++;
	i = 0;
	while (**s && (ft_isalnum(**s) || **s == '_') && i < 255)
		var_name[i++] = *(*s)++;
	var_name[i] = '\0';
	var_value = ft_getenv(var_name, all);
	if (var_value)
	{
		ft_strlcpy(*dst, var_value, ft_strlen(var_value) + 1);
		*dst += ft_strlen(var_value);
	}
	(*s)--;
}

char	*replace_env_vars(const char *s, t_all *all)
{
	char	*result;
	char	*dst;
	int		in_quote;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char) * (get_env_len(s, all) + 1));
	if (!result)
		return (NULL);
	dst = result;
	in_quote = 0;
	while (*s)
	{
		if (*s == '\'')
			in_quote = !in_quote;
		if (*s == '$' && !in_quote)
			copy_env_var(&s, &dst, all);
		else
			*dst++ = *s;
		s++;
	}
	*dst = '\0';
	return (result);
}

/******************* Exec Error ******************/

void	fd_error(char *bin_path, t_all *all)
{
	perror("fd error ");
	ft_free(bin_path);
	free_all_struct(all);
	exit(EXIT_FAILURE);
}

void	exec_error(char *bin_path, t_all *all, char *msg)
{
	ft_free(bin_path);
	free_list(all->command_list);
	free_split(all->env_arr);
	ft_free_env_list(all->env_list);
	free(all);
	if (msg)
	{
		ft_putstr_fd("Exec err: ", 2);
		ft_putstr_fd(msg, 2);
	}
	exit(EXIT_FAILURE);
}

void	command_not_found(t_list *command_arr, char *bin_path, t_all *all, char **command)
{
	if (!command_arr->next)
	{
		ft_putstr_fd("bash : ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(" : command not found...\n", 2);
	}
	free(command);
	exec_error(bin_path, all, NULL);
}

int	get_exit_stat(pid_t pids[MAX_COMMANDS], int command_count)
{
	int	status;
	int	i;

	i = -1;
	status = 0;
	while (++i < command_count)
		waitpid(pids[i], &status, 0);
	return (WEXITSTATUS(status));
}

/******************* Exec BINARY PATH ******************/

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

/******************* Exec redirection file ******************/

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

t_redirect    **get_all_redirections(char **command, t_all *all)
{
	t_redirect	**redirection_files;
	int			count;
	int			i;
	int			j;

	count = 0;
	i = -1;
	while (command[++i])
		if (command[i][0] == '>' || command[i][0] == '<')
			count++;
	if (count == 0)
		return (NULL);
	redirection_files = (t_redirect **)calloc(sizeof(t_redirect *), count + 1);
	if (!redirection_files)
		exec_error(NULL, all, "malloc error\n");
	i = -1;
	j = -1;
	while (command[++i])
	{
		if (command[i][0] == '>' || command[i][0] == '<')
		{
			redirection_files[++j] = (t_redirect *)malloc(sizeof(t_redirect));
			if (!redirection_files[j])
				exec_error(NULL, all, "malloc error\n");
			redirection_files[j]->filename = get_redirection_file_name(command[i]);
			if (command[i][0] == '>')
				redirection_files[j]->type = (command[i][1] == '>') ? APPEND : TRUNCATE;
			else
				redirection_files[j]->type = (command[i][1] == '<') ? HEREDOC : INPUT;
			if (!redirection_files[j]->filename)
				exec_error(NULL, all, "get_redirections\n");
		}
	}
	return (redirection_files);
}

static int handle_output_redirection(t_redirect *redirect, t_all *all)
{
	int fd;

	fd = 0;
	if (redirect->type == TRUNCATE)
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		fd_error(NULL, all);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

static int	handle_input_redirection(t_redirect *redirect, t_all *all)
{
	int	fd;

	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
		fd_error(NULL, all);
	if (dup2(fd, STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 failed\n");
	return (fd);
}

char	*read_join_heredoc(char *buffer, char *delimiter)
{
	char	*input;
	char	*tmp;
	char	*tmp2;

	while (1)
	{
		input = readline("heredoc> ");
		if (strcmp(input, delimiter) == 0)
		{
			free(input);
			break;
		}
		tmp = ft_strjoin(input, "\n");
		free(input);
		if (!buffer)
			buffer = ft_strdup("");
		tmp2 = buffer;
		buffer = ft_strjoin(buffer, tmp);
		free(tmp2);
		free(tmp);
	}
	return (buffer);
}

void	handle_heredoc_redirection(char *delimiter)
{
	char *buffer = NULL;

	int pipe_fds[2];
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return ;
	}
	buffer = read_join_heredoc(buffer, delimiter);
	if (buffer)
	{
		write(pipe_fds[1], buffer, strlen(buffer));
		close(pipe_fds[1]);
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(pipe_fds[0]);
			return ;
		}
		close(pipe_fds[0]);
		free(buffer);
	}
}

void	manage_redirections(t_redirect **redirections, t_all *all)
{
	int		fd;
	int		i;

	i = -1;
	if (!redirections)
		return ;
	while (redirections[++i])
	{
		fd = -1;
		if (redirections[i]->type == TRUNCATE || redirections[i]->type == APPEND)
			fd = handle_output_redirection(redirections[i], all);
		else if (redirections[i]->type == INPUT)
			fd = handle_input_redirection(redirections[i], all);
		else if (redirections[i]->type == HEREDOC)
			handle_heredoc_redirection(redirections[i]->filename);
		if (fd != -1)
			close(fd);
		ft_free(redirections[i]->filename);
		free(redirections[i]);
	}
	free(redirections);
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
	if (!new_command)
		exec_error(NULL, all, "malloc error\n");
	i = -1;
	while (command[++i])
		if (command[i][0] != '>' && command[i][0] != '<')
			new_command[++j] = command[i];
	return (new_command);
}

/******************* Exec Pipe ******************/

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

/******************* Exec non-builtings ******************/

void	exec_child(t_list *command_arr, int prev_fd[2],
			int current_fd[2], t_all *all)
{
	t_redirect	**redirections;
	char		**command;
	char		*bin_path;

	command = get_new_command((char **)command_arr->content, all);
	if (!command)
		exec_error(NULL, all, "get_new_command failed\n");
	redirections = get_all_redirections((char **)command_arr->content, all);
	manage_redirections(redirections, all);
	bin_path = get_bin_path(command[0]);
	if (!bin_path)
		command_not_found(command_arr, bin_path, all, command);
	if (prev_fd[0] != -1)
		dup_in(prev_fd, all, bin_path, 0);
	if (command_arr->next)
		dup_out(current_fd, all, bin_path, 1);
	execve(bin_path, command, all->env_arr);
	exec_error(bin_path, all, "execve failed\n");
}

void	exec_parent(t_list *command_arr, int prev_fd[2], int current_fd[2])
{
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command_arr->next)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
}

/******************* Exec builtings ******************/

int	is_builtins(t_list *command)
{
	char	**executable;

	executable = get_new_command((char **)command->content, NULL);
	if (!ft_strncmp(executable[0], "export", ft_strlen("export"))
		|| !ft_strncmp(executable[0], "unset", ft_strlen("unset"))
		|| !ft_strncmp(executable[0], "env", ft_strlen("env"))
		|| !ft_strncmp(executable[0], "echo", ft_strlen("echo"))
		|| !ft_strncmp(executable[0], "cd", ft_strlen("cd"))
		|| !ft_strncmp(executable[0], "pwd", ft_strlen("pwd"))
		|| !ft_strncmp(executable[0], "exit", ft_strlen("exit")))
		return (free(executable), 1);
	return (free(executable), 0);
}

void ft_echo(char **tokens)
{
	int	token_count;
	int	start_index;
	int skip_newline;

	token_count = array_len(tokens);
	start_index = 0;
	skip_newline = 0;
	if (token_count > 0 && strcmp(tokens[0], "echo") == 0)
	{
		if (token_count > 1 && strcmp(tokens[1], "-n") == 0)
		{
			skip_newline = 1;
			start_index = 1;
		}
		while (++start_index < token_count)
		{
			printf("%s", tokens[start_index]);
			if (start_index < token_count - 1)
				printf(" ");
		}
		if (!skip_newline)
			printf("\n");
	}
	else
		printf("Command not recognized or invalid input.\n");
}

void	restore_og_std(int std_backup[2], t_all *all)
{
	if (dup2(std_backup[0], STDIN_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdin failed\n");
	if (dup2(std_backup[1], STDOUT_FILENO) == -1)
		exec_error(NULL, all, "dup2 restore stdout failed\n");
	close(std_backup[0]);
	close(std_backup[1]);
}

void	builtin_execution(char **command, t_all *all)
{
	int	exit_status;

	if (!ft_strncmp(command[0], "export", ft_strlen("export")))
	{
		if (array_len(command) == 1)
			ft_print_export(all->env_list);
		else
			ft_export(all->env_list, command);
	}
	else if (!ft_strncmp(command[0], "unset", ft_strlen("unset")))
		ft_unset(&all->env_list, command);
	else if (!ft_strncmp(command[0], "env", ft_strlen("env")))
		ft_print_env(all->env_list);
	else if (!ft_strncmp(command[0], "echo", ft_strlen("echo")))
		ft_echo(command);
	else if (!ft_strncmp(command[0], "cd", ft_strlen("cd")))
		ft_cd(command[1], all);
	else if (!ft_strncmp(command[0], "pwd", ft_strlen("pwd")))
		ft_pwd();
	else if (!ft_strncmp(command[0], "exit", ft_strlen("exit")))
	{
		exit_status = all->exit_status;
		free_list(all->command_list);
		free_split(all->env_arr);
		ft_free_env_list(all->env_list);
		free(all);
		exit(exit_status);
	}
}

void	exec_builtins(t_list *command_list, int prev_fd[2], int current_fd[2], t_all *all)
{
	char        **command;
	t_redirect  **redirections;
	int         std_backup[2];

	std_backup[0] = dup(STDIN_FILENO);
	std_backup[1] = dup(STDOUT_FILENO);
	if (std_backup[0] == -1 || std_backup[1] == -1)
		exec_error(NULL, all, "dup backup failed\n");
	if (prev_fd[0] != -1)
		dup_in(prev_fd, all, NULL, 0);
	if (command_list->next && current_fd[1] != -1)
		dup_out(current_fd, all, NULL, 0);
	command = (char **)command_list->content;
	redirections = get_all_redirections(command, all);
	manage_redirections(redirections, all);
	builtin_execution(command, all);
	restore_og_std(std_backup, all);
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command_list->next && current_fd[1] != -1)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
}

/******************* EXEC ******************/

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
		if (is_builtins(command))
			exec_builtins(command, prev_fd, current_fd, all);
		else
		{
			pids[command_count] = fork();
			if (pids[command_count] == 0)
				exec_child(command, prev_fd, current_fd, all);
			else if (pids[command_count++] > 0)
				exec_parent(command, prev_fd, current_fd);
			else
				exec_error(NULL, all, "fork failed\n");
		}
		command = command->next;
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	all->heredoc_command = 0;
	all->exit_status = get_exit_stat(pids, command_count);
}

/******************* main ******************/

void	command_error(t_all *all)
{
	ft_putstr_fd("error : faild to allocate command\n", 1);
	free_all_struct(all);
	exit(EXIT_FAILURE);
}

int	is_valid_command(char * command)
{
	int	i;

	i = -1;
	while (command[++i])
	{
		while (ft_isspace(command[i]))
			i++;
		if (command[i] == '|' || command[i] == '\0'
			|| ft_strncmp(&command[i], ">>>", 3) == 0
			|| ft_strncmp(&command[i], "<<<", 3) == 0
			|| ft_strncmp(&command[i], "&&&", 3) == 0)
		{
			ft_putstr_fd("bash: syntax error\n", 1);
			return (0);
		}
		else
			while (command[i] && command[i] != '|')
				i++;
		if (!command[i])
			break;
	}
	return (1);
}

int	valid_command(char *command, t_all *all)
{
	int		i;
	int		is_empty;

	is_empty = 0;
	if (ft_strlen(command) == 0)
		return (0);
	i = ft_strlen(command);
	while (ft_isspace(command[--i]))
		;
	if (command[i] && command[i] == '|')
		all->heredoc_command = 1;
	if (!is_valid_command(command))
	{
		free(command);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	**commands;
	char	*line;
	t_all	*all;

	(void) argc;
	(void) argv;
	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (0);
	all->exit_status = 0;
	all->command_list = NULL;
	all->env_list = NULL;
	all->heredoc_command = 0;
	int_lst_env(&all->env_list, envp);
	all->env_arr = list_to_array(all->env_list);
	while (1)
	{
		line = readline(">: ");
		if (*line)
			add_history(line);
		line = replace_env_vars(line, all);
		commands = ft_split_esc(line, '|');
		print_split(commands);
		printf("line = '%s'\n", line);
		if (valid_command(line, all))
		{
			ft_free(line);
			init_list(&all->command_list, commands);
			exec_commands(all);
			free_list(all->command_list);
		}
	}
	return (free_all_struct(all),  0);
}

/* signal : ctrl-C, ctrl-D, ctrl-\ */
// shellevel
// ls | (heredoc command) -> ft_split_esc(line, '|') -> all->heredoc_command
// 
// export VAR && export VAR

// cat << (heredoc)
// export list=ls ; $list
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
