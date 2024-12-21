/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/12/21 15:42:38 by safandri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>
# include <signal.h>
# include <errno.h>
# include <dirent.h>

# include "libft/libft.h"

extern volatile int	g_flag;

# define MAX_COMMANDS 1000
# define MAX_VAR_LEN 256

# define TRUNCATE 1
# define APPEND 2
# define INPUT 3
# define HEREDOC 4

typedef struct s_env_list
{
	char				*value;
	int					type;
	char				*first;
	char				*second;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_redirect
{
	char		*filename;
	int			type;
	int			fd;
}				t_redirect;

typedef struct s_all
{
	long long	exit_status;
	t_list		*command_list;
	char		**env_arr;
	int			fd_og[2];
	int			in_pipe[2];
	int			out_pipe[2];
	pid_t		pids[MAX_COMMANDS];
	char		**command;
	char		*bin_path;
	t_redirect	**redir;
	t_env_list	*env_list;
}				t_all;

/******************** CMD VALIDATOR ****************** */

int				is_symbol(char c);
int				symbols_condition(const char *command, int i);
int				ft_check_quote(const char *command, char cote, int *i);
int				check_space_and_symbols(const char *command, int *i);
int				check_symbol(const char *command, char c, int *i);
int				handle_pipe(int *flag_redirection, const char *command, int *i);
int				handle_redirection(int *flag_redirection,
					const char *command, int *i);
int				print_syntax_error(int *flag_redirection);
int				check_unlosed_quotes(char *command, int i);
int				error_checker_loop(const char *command, int i,
					int *flag_redirection);
int				valid_command(const char *command, t_all *all);
int				is_valid_command(const char *command);

/******************** ENV PARSER ****************** */

char			*ft_getenv(char *env_var, t_all *all);
char			*get_env_name(char *s);
char			*get_env_value(char *var_name, t_all *all);
char			*copy_char(char *dest, char c);
void			manage_env_var(char **dst, char **s, t_all *all);
void			append_env_value(char **dst, char **s, t_all *all);
void			append_env_value_quote(char **dst, char **s, t_all *all);
void			append_quoted_text(char **dst, char **s,
					char quote, t_all *all);
int				if_path(char *s);
char			*expand_path(char **s, char **dst, t_all *all);
char			*handle_quoted_delimiter(char **s, char *dst, char quote_type);
char			*handle_heredoc_delimiter(char **s, char *dst);
int				initiate_dst(char **dst, char *s);

/******************** SPLIT 2 ****************** */

void			*ft_free_exit(char **tab);
char			*ft_escape_quote(char *s);
int				is_special_char(char c);
char			*ft_escap_spliter(char *s, char c);
char			*ft_escape_special_char(char *s);
size_t			ft_count_words_2(char *s, char c);
size_t			ft_count_words_len_2(char *s, char c);
void			skip_quote(char *s, int *word_len, int *i);
void			handle_special_char(char *s, int *word_len, int *i);
size_t			ft_count_str_len(char *s, char c);
void			cpy_to_arr(char *s, char *tab, int word_len);
char			**ft_split_esc_2(char *s, char c);
char			*replace_env_vars(char *s, t_all *all);

int				arg_skip_quote(char *s);
int				arg_special_char(char *s);
size_t			ft_arg_type_words_len(char *s, char c);
int				*ft_split_arg_type(char *s, char c);

/******************** UTILS ****************** */

void			free_split(char **array);
void			print_split(char **array);
int				array_len(char **array);
void			ft_free(void *s);
void			print_s(void *s);
void			free_split_1(void *s);
void			split_iterate(void **array, void (*f)(void *));
void			print_list(void *s);
void			print_command_list(t_list *commands);
void			free_list(t_list *s);
void			free_all_struct(t_all *all);

/******************** INIT ALL STUCT ****************** */

void			list_init_error(t_list *commands_list, char **arr_commands);
void			init_list(t_list **commands_list, char **arr_commands);
void			init_all_struct(t_all *all, char **envp);

/******************** env builtins ****************** */

void			ft_swap_string(char **s1, char **s2);
void			ft_sort_list(t_env_list *env);
void			join_first_second(int flag, t_env_list *new);
t_env_list		*ft_lstnew(char *content, int flag);
t_env_list		*ft_lstlast(t_env_list *lst);
void			ft_lstadd_back(t_env_list **lst, t_env_list *new_element);
void			ft_free_env_list(t_env_list *env);
void			free_shlvl(char *content);
t_env_list		*ft_lstnew_init(char *content, int flag);
void			int_lst_env(t_env_list **list, char **envp);
char			**list_to_array(t_env_list *env);
int				ft_print_env(t_env_list *env);

/******************** EXPORT ****************** */

int				ft_maj_export(t_env_list *env, char *command,
					int *flag, int *return_value);
int				check_env_error(char *command,
					char *variable_name, int *return_value);
void			ft_update_ensemble(int *flag, t_env_list *tmp,
					char *line, char *cmd);
void			ft_update_flag_1(t_env_list *tmp, char *line);
void			ft_update_flag_0(t_env_list *tmp, char *line, char *commande);
int				ft_print_export(t_env_list *env);
int				ft_export_error(char *variable_name, char *commande);
t_env_list		*ft_dup_env(t_env_list **env);
void			ft_print_export_error(char *commande);
int				ft_check_after_first_caracter(char *var_name, char *cmd);
char			*ft_init_variable_name(char *commande, int *flag, char *line);
int				ft_export(t_env_list **env, char **commade);

/******************** UNSET ****************** */

int				ft_unset(t_env_list **env, char **commande);
void			ft_free_tmp(t_env_list *tmp);

/****************** BUILTINS ******************** */

int				ft_pwd(void);
int				ft_cd(char **path, t_all *all);
int				ft_echo(char **tokens);
int				ft_exit(t_all *all, char **command);

/******************* EXEC ******************* */

int				is_builtins(char *command);
int				exec_builtins(t_list *command_list, int in_pipe[2],
					int out_pipe[2], t_all *all);
int				builtin_execution(char **command, t_all *all);

void			restore_og_std(int std_backup[2], t_all *all);
int				invalid_file(DIR *dir, char *command, t_all *all);
char			**get_new_command(t_list *command_list, t_all *all);
void			close_dir(DIR *dir, char *path);
int				is_dir(char *command, t_all *all);
char			*get_first_command(t_list *command_list);
void			restore_std(t_all *all);
int				builtins_output_redirection(t_redirect *redirect, t_all *all);
int				builtin_redirections(t_all *all);
int				exec_commands(t_all *all);
void			init_list(t_list **commands_list, char **arr_commands);
char			**ft_split_esc(const char *s, char c);
void			get_all_exit_stat(t_all *all, int command_count);
void			exec_parent(t_all *all, t_list *command_list);
void			command_dir_error(t_all *all);
void			get_command_bin(t_all *all, t_list *command_list);
void			free_if_failed_exec(t_all *all);
void			exec_child(t_all *all, t_list *command_list);
void			exec_non_forked(t_all *all, t_list *command_list);
void			init_t_all_var(t_all *all);
int				manage_heredoc_sigdef(t_all *all);
void			exec_forked(t_all *all, t_list *command_list,
					int command_count);

/****************** BIN PATH ********************/

char			*join_bin_path(char *commands_list, char *bin_path);
int				command_isdir(char *command, t_all *all);
char			*find_bin(char **bin_paths, char *command);
char			*get_bin_path(char *command, t_all *all);

/****************** REDIR ********************/

char			*get_redir_name(char *command);
t_redirect		**get_all_redirections(t_list *command_list, t_all *all);
int				handle_output_redirection(t_redirect *redirect,
					t_all *all, t_redirect **redir_head);
int				manage_redirections(t_redirect	**redir, t_all *all);
void			dup_in(int fd[2], int closeall);
void			dup_out(int fd[2], int closeall);

/****************** HEREDOC ********************/

int				get_heredoc(char *delimiter, int *fd, t_all *all);
int				get_builtin_heredoc(char *delimiter, t_all *all);
char			*read_join_heredoc(char *buffer, char *delimiter,
					int pipe_fd[2], t_all *all);
int				handle_ctrl_d(char *delimiter, char *input);
void			handle_hered_singint(t_all *all, int pipe_fd[2], char *buffer);
int				handle_heredoc_redirection(int fd);
int				handle_input_redirection(t_redirect *redirect,
					t_all *all, t_redirect **redir_head);
char			*join_result(char *buffer, char *input,
					int replace_env, t_all *all);

void			exec_error(char *bin_path, t_all *all, char *msg);
int				command_not_found(t_all *all);
int				command_not_found_point(t_all *all);
void			fd_error(char *file_path, t_redirect **redir, t_all *all);
void			free_all_redir(t_redirect **redir);
char			*new_delimiter(char *old, int *del_quote);

/****************** SIGNAL ********************/

void			put_signal_handlig(int i);
void			handle_ctrl_c_heredoc(int sig, siginfo_t *ok, void *param);

#endif