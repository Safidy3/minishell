/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: safandri <safandri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:23:39 by safandri          #+#    #+#             */
/*   Updated: 2024/10/17 13:10:31 by safandri         ###   ########.fr       */
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

# define MAX_COMMANDS 100

enum e_typage
{
	STRING,
	R_OUT,
	R_IN,
	R_PIPE,
	RR_IN,
	RR_OUT,
	PIPE_FILE,
	INPUT_FILE,
	OUTPUT_FILE,
};

typedef struct s_command_list
{
	char *value;
	int type;
	char *first;
	char *second;
	struct s_command_list *next;

}t_command_list;

typedef struct s_all
{
	char	**env;
	int		exit_status;
	t_list	*command_list;
}			t_all;

char	**ft_split_esc_2(char *s, char c);

/************************************** */


void			ft_swap_string(char **s1, char **s2);
void			ft_sort_list(t_command_list *env);
void			join_first_second(int flag, t_command_list *new);
t_command_list	*ft_lstnew(char *content, int flag);
t_command_list	*ft_lstlast(t_command_list *lst);
void			ft_lstadd_back(t_command_list **lst, t_command_list *new_element);



/************************************** */


void	int_lst_env(t_command_list **list,char **envp);
char	**list_to_array(t_command_list *env);
void	ft_print_env(t_command_list *env);


/************************************** */

int				ft_strcmp(const char *s1, const char *s2);
void			ft_print_export_error(char *commande);
int				ft_check_after_first_caracter(char *variable_name, char *commande);
int				ft_export_error(char *variable_name, char *commande);
t_command_list	*ft_dup_env(t_command_list **env);
void			ft_prin_export(t_command_list *env);
void			ft_update_flag_1(t_command_list *tmp, char *line);
void			ft_update_flag_0(t_command_list *tmp, char *line, char *commande);
void			ft_update_ensemble(int *flag, t_command_list *tmp, char *line, char *commande);
char			*ft_init_variable_name(char *commande, int *flag, char *line);
void			update_final_export(char *line, t_command_list *tmp, char *variable_name, int *flag, char *commande);
int				ft_maj_export(t_command_list *env, char *commande, int *flag);
void			ft_export(t_command_list *env, char **commade);


/************************************** */

void			ft_free_tmp(t_command_list *tmp);
void			ft_unset(t_command_list **env, char **commande);

#endif