/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/11/16 15:53:16 by safandri         ###   ########.fr       */
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
# include "libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>


# include <string.h>

# define MAX_COMMANDS 100

# define TRUNCATE 1
# define APPEND 2
# define INPUT 3
# define HEREDOC 4

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
	int			exit_status;
	t_list		*command_list;
	char		**env_arr;
	t_env_list	*env_list;
}				t_all;

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
}				t_redirect;


char			**ft_split_esc_2(char *s, char c);

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
void			ft_export(t_env_list *env, char **commade);

/************************************** */

void			ft_free_tmp(t_env_list *tmp);
void			ft_unset(t_env_list **env, char **commande);

/************************************** */

void			ft_pwd();
void			ft_cd(char *path, t_all *all);


#endif