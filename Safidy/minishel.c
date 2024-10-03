#include "minishel.h"

// ls -la | grep mini | awk '{print $9}' | head -n 1
/*
color in C

    printf("\033[1;31mThis is red text\033[0m\n");
    printf("\033[1;32mThis is green text\033[0m\n");
    printf("\033[1;33mThis is yellow text\033[0m\n");
    printf("\033[1;34mThis is blue text\033[0m\n");
    printf("\033[1;35mThis is magenta text\033[0m\n");
    printf("\033[1;36mThis is cyan text\033[0m\n");

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

void	free_split(void *s)
{
	free((char *) s);
}

void	print_split(void *s)
{
	printf("%s \n", (char *) s);
}

void	split_iterate(void **array, void (*f)(void *))
{
	int	i;

	i = -1;
	while (array[++i])
		f(array[i]);
}

/******************* list iteration ******************/

void	print_list(void *s)
{
	split_iterate((void *) s, print_split);
	printf("\n");
}

void	free_list_content(void *s)
{
	if (!s)
		return ;
	split_iterate(s, free_split);
	free(s);
}

void	free_list(t_list *s)
{
	t_list *temp;

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
	split_iterate((void **) arr_commands, free_split);
	free(arr_commands);
	free_list(commands_list);
	printf("\033[1;31mAllocation error :\033[0m\n faild to allocate the ");
	exit(EXIT_FAILURE);
}

void	initiate_list(t_list **commands_list, char **arr_commands)
{
	int		i;
	t_list	*new_list;

	i = -1;
	while (arr_commands[++i])
	{
		new_list = ft_lstnew((void *) ft_split_esc(arr_commands[i], ' '));
		if (new_list == NULL)
			list_init_error(*commands_list, arr_commands);
		if (i == 0)
			*commands_list = new_list;
		else
			ft_lstadd_back(commands_list, new_list);
	}
}

/******************* main ******************/

int main()
{
	char	*example_com;
	char	**commands;
	t_list	*commands_list;

	commands_list = NULL;
	example_com = "ls -la | grep mini | awk '{print $9}' | head -n 1";
	commands = ft_split(example_com, '|');

	printf("%s\n\n", example_com);


	initiate_list(&commands_list, commands);
	ft_lstiter(commands_list, print_list);


	free_list(commands_list);
	split_iterate((void **) commands, free_split);
	free(commands);

}