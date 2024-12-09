/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/12/10 15:10:09 by safandri         ###   ########.fr       */
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
# include <string.h>

extern volatile int	flag;

# define MAX_COMMANDS 100
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

typedef struct s_all
{
	int			exit_status;
	t_list		*command_list;
	char		**env_arr;
	int			fd_og[2];
	int			prev_fd[2];
	int			current_fd[2];
	t_env_list	*env_list;
}				t_all;

typedef struct s_redirect
{
	char		*filename;
	int			type;
	int			fd;
}				t_redirect;

typedef struct s_cmd_utils
{
	pid_t		pids[MAX_COMMANDS];
	int			exit_stats[MAX_COMMANDS];
	int			cmd_type[MAX_COMMANDS];
	char		**cmd;
	char		*bin_path;
}				t_cmd_utils;

/******************** PARSING ****************** */

char			**ft_split_esc_2(char *s, char c);
int				*ft_split_arg_type(char *s, char c);

/******************** UTILS ****************** */

void			free_split(char **array);
void			print_split(char **array);
char			*ft_getenv(char *env_var, t_all *all);
int				array_len(char **array);
void			ft_free(char *s);
void			print_s(void *s);
void			free_split_1(void *s);
void			split_iterate(void **array, void (*f)(void *));
void			print_list(void *s);
void			print_command_list(t_list *commands);
void			free_list(t_list *s);
void			free_all_struct(t_all *all);

/************************************** */

int				valid_command(char *command, t_all *all);
int				is_valid_command(char *command);

/************************************** */

void			ft_swap_string(char **s1, char **s2);
void			ft_sort_list(t_env_list *env);
void			join_first_second(int flag, t_env_list *new);
t_env_list		*ft_lstnew(char *content, int flag);
t_env_list		*ft_lstlast(t_env_list *lst);
void			ft_lstadd_back(t_env_list **lst, t_env_list *new_element);
void			ft_free_env_list(t_env_list *env);

/***************** ENV ********************* */

void			int_lst_env(t_env_list **list, char **envp);
char			**list_to_array(t_env_list *env);
char			*ft_getenv(char *env_var, t_all *all);
int				ft_print_env(t_env_list *env);

/******************** EXPORT ****************** */

int				ft_strcmp(const char *s1, const char *s2);
void			ft_print_export_error(char *commande);
int				ft_check_after_first_caracter(char *var_name, char *cmd);
int				ft_export_error(char *variable_name, char *commande);
t_env_list		*ft_dup_env(t_env_list **env);
int				ft_print_export(t_env_list *env);
void			ft_update_flag_1(t_env_list *tmp, char *line);
void			ft_update_flag_0(t_env_list *tmp, char *line, char *commande);
void			ft_update_ensemble(int *flag, t_env_list *tmp,
					char *line, char *cmd);
char			*ft_init_variable_name(char *commande, int *flag, char *line);
int				ft_maj_export(t_env_list *env, char *commande, int *flag);
int				ft_export(t_env_list **env, char **commade);
void			ft_free_tmp(t_env_list *tmp);
int				ft_unset(t_env_list **env, char **commande);

/************************************** */
char			*ft_getenv(char *env_var, t_all *all);

/****************** BUILTINS ******************** */

int				ft_pwd(void);
int				ft_cd(char **path, t_all *all);
int				ft_echo(char **tokens);
int				ft_exit(t_all *all, char **command);

/****************** EXEC BUILTINS ******************** */

int				is_builtins(char *command);
int				exec_builtins(t_list *command_list, int prev_fd[2],
					int current_fd[2], t_all *all);
int				builtin_execution(char **command, t_all *all);

/******************* EXEC ******************* */

void			restore_og_std(int std_backup[2], t_all *all);
char			**get_new_command(t_list *command_list, t_all *all);
void			close_dir(DIR *dir, char *path);
char			*replace_env_vars(char *s, t_all *all);
int				exec_commands(t_all *all);
void			init_list(t_list **commands_list, char **arr_commands);
char			**ft_split_esc(const char *s, char c);
int				get_exit_stat(pid_t pids[MAX_COMMANDS], int command_count);

/****************** BIN PATH ********************/

char			*join_bin_path(char *commands_list, char *bin_path);
char			*get_bin_path(char *command, t_all *all);

/****************** REDIR ********************/

char			*get_redir_name(char *command);
t_redirect		**get_all_redirections(t_list *command_list, t_all *all);
int				handle_output_redirection(t_redirect *redirect, t_all *all);
int				manage_redirections(t_list *command_list, t_all *all);
void			dup_in(int fd[2], int closeall);
void			dup_out(int fd[2], int closeall);

/****************** HEREDOC ********************/

int				get_heredoc(char *delimiter, int *fd, t_all *all);
int				handle_input_redirection(t_redirect *redirect, t_all *all);

/****************** EXEC ERROR ********************/

void			exec_error(char *bin_path, t_all *all, char *msg);
int				command_not_found(t_list *command_list, char **command);
void			fd_error(char *file_path);
int				handle_heredoc_redirection(int fd);

/**************************************/

void			put_signal_handlig(int i);
void			handle_ctrl_c_heredoc(int sig, siginfo_t *ok, void *param);

#endif