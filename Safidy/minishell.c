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

char *ft_getenv(char *env_var, t_all *all)
{
	t_env_list *tmp;
	char *res;

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

char *get_env_name(char *s)
{
	char *var_name;
	size_t len;
	size_t i;

	if (*s != '$')
		return (NULL);
	s++;
	i = 0;
	len = -1;
	while (s[++len] && ft_isdigit(s[len]))
		;
	while (s[++len] && ft_isalnum(s[len]))
		i++;
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	var_name[len] = '\0';
	i = -1;
	while (s[++i] && i < len)
		var_name[i] = s[i];
	return (var_name);
}

char *get_env_value(char *var_name, t_all *all)
{
	char *var_value;

	var_value = NULL;
	if (var_name)
		var_value = ft_getenv(var_name, all);
	return (var_value);
}

char *copy_char(char *dest, char c)
{
	char *res;
	int len;

	if (!dest)
		return (NULL);
	len = ft_strlen(dest);
	res = malloc(sizeof(char) * (len + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, dest, len + 1);
	res[len] = c;
	res[len + 1] = '\0';
	return (free(dest), res);
}

void append_env_value(char **dst, char **s, t_all *all)
{
	char *env_name;
	char *env_val;
	char *temp;
	char *exit_stat;

	if (**s == '$' && ft_isalnum(*(*s + 1)))
	{
		env_name = get_env_name(*s);
		env_val = get_env_value(env_name, all);
		if (env_val)
		{
			temp = *dst;
			*dst = ft_strjoin(*dst, env_val);
			free(temp);
			free(env_val);
		}
		else if (ft_isdigit(*(*s + 1)))
		{
			temp = *dst;
			*dst = ft_strjoin(*dst, &env_name[1]);
			free(temp);
		}
		while (++(*s) && ft_isalnum(**s))
			;
		ft_free(env_name);
	}
	else if (**s == '$' && *(*s + 1) == '?' && (ft_isspace(*(*s + 1)) || !(*(*s + 1)) || !ft_isalnum(*(*s + 2))))
	{
		exit_stat = ft_itoa(all->exit_status);
		temp = *dst;
		*dst = ft_strjoin(*dst, exit_stat);
		free(temp);
		free(exit_stat);
		*s += 2;
	}
	else if (**s == '$' && (ft_isspace(*(*s + 1)) || !(*(*s + 1)) || !ft_isalnum(*(*s + 2))))
		*dst = copy_char(*dst, *(*s)++);
}

void append_quoted_text(char **dst, char **s, char quote, t_all *all)
{
	*dst = copy_char(*dst, *(*s)++);
	while (**s && **s != quote)
	{
		if (**s == '$' && quote == '\"')
			append_env_value(dst, s, all);
		else
			*dst = copy_char(*dst, *(*s)++);
	}
	if (*s && **s == quote)
		*dst = copy_char(*dst, *(*s)++);
}

char *replace_env_vars(char *s, t_all *all)
{
	char *dst;
	char *temp;

	dst = ft_strdup("");
	if (!dst)
		return (NULL);
	while (*s)
	{
		if (*s == '\'')
			append_quoted_text(&dst, &s, '\'', all);
		else if (*s == '\"')
			append_quoted_text(&dst, &s, '\"', all);
		else if (*s == '$')
			append_env_value(&dst, &s, all);
		else if (*s == '~' && (!*(s + 1) || ft_isspace(*(s + 1)) || *(s + 1) == '/') && (!*(s - 1) || ft_isspace(*(s - 1))))
		{
			temp = dst;
			dst = ft_strjoin(dst, ft_getenv("HOME", all));
			free(temp);
			s++;
		}
		else
			dst = copy_char(dst, *s++);
	}
	return (dst);
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

int command_not_found(t_list *command_list, char **command)
{
	if (!command_list->next)
	{
		ft_putstr_fd("bash: line 1: ", 2);
		ft_putstr_fd(command[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free(command);
	return (127);
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

/******************* HEREDOC ******************/

void handle_heredoc_redirection(int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return;
	}
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

char *read_join_heredoc(char *buffer, char *delimiter, t_all *all)
{
	char *input;
	char *tmp;
	char *tmp2;
	(void)all;

	tmp2 = NULL;
	tmp = NULL;
	input = NULL;
	while (1)
	{
		put_signal_handlig(2);
		input = readline("heredoc> ");
		if (flag == SIGINT && !input)
		{
			put_signal_handlig(1);
			flag = 0;
			dup2(all->fd_og[0], STDIN_FILENO);
			ft_free(input);
			ft_free(tmp);
			ft_free(tmp2);
			return (NULL);
		}
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

int get_heredoc(char *delimiter, int *fd, t_all *all)
{
	char *buffer = NULL;

	int pipe_fds[2];
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	buffer = read_join_heredoc(buffer, delimiter, all);
	if (buffer)
	{
		write(pipe_fds[1], buffer, strlen(buffer));
		close(pipe_fds[1]);
		*fd = dup(pipe_fds[0]);
		close(pipe_fds[0]);
		free(buffer);
		return (0);
	}
	else
		return (1);
	return (0);
}

int manage_redirections(t_redirect **redirections, t_all *all)
{
	int fd;
	int i;

	if (!redirections)
		return (0);
	i = -1;
	while (redirections[++i])
		if (redirections[i]->type == HEREDOC)
			if (get_heredoc(redirections[i]->filename, &redirections[i]->fd, all))
				return (1);
	i = -1;
	while (redirections[++i])
	{
		fd = -1;
		if (redirections[i]->type == TRUNCATE || redirections[i]->type == APPEND)
			fd = handle_output_redirection(redirections[i], all);
		else if (redirections[i]->type == INPUT)
			fd = handle_input_redirection(redirections[i], all);
		else if (redirections[i]->type == HEREDOC)
			handle_heredoc_redirection(redirections[i]->fd);
		if (fd != -1)
			close(fd);
		ft_free(redirections[i]->filename);
		free(redirections[i]);
	}
	free(redirections);
	return (0);
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

void	exec_parent(t_list *command_list, int prev_fd[2], int current_fd[2])
{
	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command_list->next)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
}

/******************* Exec builtings ******************/

int	is_builtins(char *command)
{
	if (!ft_strncmp(command, "export", ft_strlen("export"))
		|| !ft_strncmp(command, "unset", ft_strlen("unset"))
		|| !ft_strncmp(command, "env", ft_strlen("env"))
		|| !ft_strncmp(command, "echo", ft_strlen("echo"))
		|| !ft_strncmp(command, "cd", ft_strlen("cd"))
		|| !ft_strncmp(command, "pwd", ft_strlen("pwd"))
		|| !ft_strncmp(command, "exit", ft_strlen("exit")))
		return (1);
	return (0);
}

int	is_echo_flag(char *flag)
{
	if (*flag != '-')
		return (0);
	flag++;
	while (*flag)
	{
		if (*flag != 'n')
			return (0);
		flag++;
	}
	return (1);
}

int	ft_echo(char **tokens)
{
	int	token_count;
	int	start_index;
	int	skip_newline;
	int	i;

	token_count = array_len(tokens);
	start_index = 0;
	skip_newline = 0;
	i = 0;
	if (token_count > 0 && strcmp(tokens[0], "echo") == 0)
	{
		if (token_count > 1)
		{
			while (is_echo_flag(tokens[++i]))
			{
				start_index++;
				skip_newline = 1;
			}
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
	return (0);
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

int	builtin_execution(char **command, t_all *all)
{
	int	exit_status;

	exit_status = 0;
	if (!ft_strncmp(command[0], "export", ft_strlen("export")))
	{
		if (array_len(command) == 1)
			ft_print_export(all->env_list);
		else
			exit_status = ft_export(all->env_list, command);
	}
	else if (!ft_strncmp(command[0], "unset", ft_strlen("unset")))
		ft_unset(&all->env_list, command);
	else if (!ft_strncmp(command[0], "env", ft_strlen("env")))
		ft_print_env(all->env_list);
	else if (!ft_strncmp(command[0], "echo", ft_strlen("echo")))
		exit_status = ft_echo(command);
	else if (!ft_strncmp(command[0], "cd", ft_strlen("cd")))
		exit_status = ft_cd(command[1], all);
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
	return (exit_status);
}

int	exec_builtins(t_list *command_list, int prev_fd[2], int current_fd[2], t_all *all)
{
	char        **command;
	int			exit_stat;

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
	exit_stat = builtin_execution(command, all);
	restore_og_std(std_backup, all);

	if (prev_fd[0] != -1)
		close(prev_fd[0]);
	if (command_list->next && current_fd[1] != -1)
	{
		close(current_fd[1]);
		prev_fd[0] = current_fd[0];
	}
	return (exit_stat);
}

/******************* EXEC ******************/

/* ls -la | grep Nov | cat | wc -l */

int exec_commands(t_all *all)
{
	t_list *command_list;
	pid_t pids[MAX_COMMANDS];
	int exit_stats[MAX_COMMANDS];
	int prev_fd[2];
	int current_fd[2];
	int command_count;

	t_redirect **redirections;
	char **command;
	char *bin_path;

	command_count = 0;
	prev_fd[0] = -1;
	prev_fd[1] = -1;

	command_list = all->command_list;
	command_count = ft_lstsize(all->command_list);

	command_count = 0;
	command_list = all->command_list;
	while (command_list)
	{
		if (command_list->next && pipe(current_fd) == -1)
			exec_error(NULL, all, "pipe creation failed\n");

		redirections = get_all_redirections((char **)command_list->content, all);
		if (manage_redirections(redirections, all))
			return (-1);
		command = get_new_command((char **)command_list->content, all);
		if (!command)
			exec_error(NULL, all, "get_new_command failed\n");

		bin_path = get_bin_path(command[0]);
		if (!bin_path && !is_builtins(command[0]))
			exit_stats[command_count] = command_not_found(command_list, command);
		if (ft_strcmp(command[0], "exit") == 0)
		{
			ft_putendl_fd(command[0], 2);
			int exit_status = all->exit_status;
			free_list(all->command_list);
			free_split(all->env_arr);
			ft_free_env_list(all->env_list);
			free(all);
			free(command);
			exit(exit_status);
		}
		else
		{
			pids[command_count] = fork();
			if (pids[command_count] == 0)
			{
				if (prev_fd[0] != -1)
					dup_in(prev_fd, all, bin_path, 0);
				if (command_list->next)
					dup_out(current_fd, all, bin_path, 1);
				if (bin_path)
					execve(bin_path, command, all->env_arr);
				else if (is_builtins(command[0]))
				{
					exit_stats[command_count] = builtin_execution(command, all);
					free_all_struct(all);
					free(command);
					exit(exit_stats[command_count]);
				}
				exec_error(bin_path, all, "execve failed\n");
			}
			else if (pids[command_count] > 0)
			{
				if (prev_fd[0] != -1)
					close(prev_fd[0]);
				if (command_list->next)
				{
					close(current_fd[1]);
					prev_fd[0] = current_fd[0];
				}

				dup2(all->fd_og[0], STDIN_FILENO);
				dup2(all->fd_og[1], STDOUT_FILENO);
				int status;
				waitpid(pids[command_count], &status, 0);
				exit_stats[command_count] = WEXITSTATUS(status);
				ft_free(bin_path);
				free(command);
			}
			else
				exec_error(NULL, all, "fork failed\n");
		}
		command_list = command_list->next;
		command_count++;
	}
	if (prev_fd[0] != -1)
		close(prev_fd[0]);

	int i = 0;
	while (i < command_count)
		all->exit_status = exit_stats[i++];
	return (0);
}

/******************* command validity ******************/

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
	if (ft_strlen(command) == 0)
		return (0);
	if (!is_valid_command(command))
	{
		all->exit_status = 2;
		free(command);
		return (0);
	}
	return (1);
}

/******************* Signal ******************/

volatile int flag;

void handle_ctrl_c_heredoc(int sig, siginfo_t *ok, void *param)
{

	if (ok->si_pid != 0 && sig == SIGINT)
	{
		flag = SIGINT;
		printf("\n");
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
	param++;
}

void handle_ctrl_c(int sig, siginfo_t *ok, void *param)
{
	if (ok->si_pid != 0 && sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	flag = SIGINT;
	param++;
}

void put_signal_handlig(int i)
{
	struct sigaction a;

	ft_bzero(&a, sizeof(sigaction));
	if (i == 0)
		a.sa_handler = SIG_IGN;
	else if (i == 1)
		a.sa_sigaction = handle_ctrl_c;
	else if (i == 2)
		a.sa_sigaction = handle_ctrl_c_heredoc;
	sigemptyset(&a.sa_mask);
	a.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &a, NULL) < 0)
	{
		return;
	}
	signal(SIGQUIT, SIG_IGN);
}

/******************* main ******************/

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
		put_signal_handlig(1);
		line = readline(">: ");
		if (!line)
		{
			write(1, "read line error\n", 16);
			exit(all->exit_status);
		}
		if (*line)
			add_history(line);
		line = replace_env_vars(line, all);

		// printf("line : %s\n", line);

		commands = ft_split_esc(all, line, '|');

		// printf("commands :\n");
		// print_split(commands);

		if (valid_command(line, all))
		{
			ft_free(line);
			init_list(&all->command_list, commands);

			// printf("command_list :\n\n");
			// print_command_list(all->command_list);
			// printf("\n\n");

			if (exec_commands(all) == -1)
			{
				free_list(all->command_list);
				continue;
			}
			free_list(all->command_list);
			// printf("\nexit_status : %d\n\n", all->exit_status);
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
// 	all->heredoc_command = 0;
// 	int_lst_env(&all->env_list, envp);
// 	all->env_arr = list_to_array(all->env_list);

// 	line = "export TES!T=123";
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
// 	all->heredoc_command = 0;
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
// 	all->heredoc_command = 0;
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