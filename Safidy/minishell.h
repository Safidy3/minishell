/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/11/29 14:57:29 by safandri         ###   ########.fr       */
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

# include "libft/libft.h"
# include "gnl/get_next_line.h"

# include <string.h>

extern volatile int flag;

# define MAX_COMMANDS 100
# define MAX_VAR_LEN 256

# define TRUNCATE 1
# define APPEND 2
# define INPUT 3
# define HEREDOC 4

typedef struct s_cmd_list
{
	char				**cmd_arr;
	int					*in_quote;
	struct s_cmd_list	*next;
}						t_cmd_list;

typedef struct	s_env_list
{
	char		*value;
	int			type;
	char		*first;
	char		*second;
	struct		s_env_list *next;
}				t_env_list;

typedef struct	s_all
{
	int				exit_status;
	t_cmd_list		*command_list;
	char			**env_arr;
	int				heredoc_command;
	t_env_list		*env_list;
}					t_all;

/*
	1:'>' truncate output
	2:'>>' append output
	3:'<' input
	4:'<<' heredoc
*/
typedef struct	s_redirect
{
	char		*filename;
	int			type;
	int			fd;
}				t_redirect;

/************************************** */


t_cmd_list		*ft_lst_new_cmd(char **cmd_arr);
void			ft_lst_cmd_iter(t_cmd_list *lst, void (*f)(char **));
void			ft_lst_cmd_clear(t_cmd_list **lst, void (*del)(char **));
void			ft_lst_cmd_delone(t_cmd_list *lst, void (*del)(char **));
void			ft_lst_add_back_cmd(t_cmd_list **lst, t_cmd_list *new);
int				ft_lst_cmd_size(t_cmd_list *lst);

/************************************** */

char			**ft_split_esc_2(char *s, char c);
int				*ft_split_arg_type(char *s, char c);

/************************************** */

void			ft_swap_string(char **s1, char **s2);
void			ft_sort_list(t_env_list *env);
void			join_first_second(int flag, t_env_list *new);
t_env_list		*ft_lstnew(char *content, int flag);
t_env_list		*ft_lstlast(t_env_list *lst);
void			ft_lstadd_back(t_env_list **lst, t_env_list *new_element);
void			ft_free_env_list(t_env_list *env);

/************************************** */

void			int_lst_env(t_env_list **list,char **envp);
char			**list_to_array(t_env_list *env);
char			*ft_getenv(char *env_var, t_all *all);
void			ft_print_env(t_env_list *env);

/************************************** */

int				ft_strcmp(const char *s1, const char *s2);
void			ft_print_export_error(char *commande);
int				ft_check_after_first_caracter(char *variable_name, char *commande);
int				ft_export_error(char *variable_name, char *commande);
t_env_list		*ft_dup_env(t_env_list **env);
void			ft_print_export(t_env_list *env);
void			ft_update_flag_1(t_env_list *tmp, char *line);
void			ft_update_flag_0(t_env_list *tmp, char *line, char *commande);
void			ft_update_ensemble(int *flag, t_env_list *tmp, char *line, char *commande);
char			*ft_init_variable_name(char *commande, int *flag, char *line);
int				ft_maj_export(t_env_list *env, char *commande, int *flag);
int				ft_export(t_env_list *env, char **commade);

/************************************** */

void			ft_free_tmp(t_env_list *tmp);
void			ft_unset(t_env_list **env, char **commande);

/************************************** */

void			ft_pwd();
int				ft_cd(char *path, t_all *all);

/************************************** */

char			*replace_env_vars(char *s, t_all *all);
void			exec_commands(t_all *all);
void			init_list(t_cmd_list **commands_list, char **arr_commands);
char			**ft_split_esc(t_all *all, const char *s, char c);

#endif