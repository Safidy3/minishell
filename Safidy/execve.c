#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct command
{
	char	*path;
	char	**argument;
	char	**environ;
}				command;


void	set_command(command	*c, char *path, char **argument, char **environ)
{

	c->path = path;
	c->argument = argument;
	c->environ = environ;
}


// int main(int argc, char *argv[])
int main(void)
{
	int	pid;

	char	*ls_path = "/usr/bin/ls";
	char	*ls_argv[] = { ls_path, "-l", "-a", NULL };
	char	*env[] = { NULL };

	char	*grep_path = "/usr/bin/grep";
	char	*grep_argv[] = { grep_path, "zip", NULL };

	pid = fork();
	if (pid == 0)
		execve(ls_path, ls_argv, env);
	else
		waitpid(pid, NULL, 0);


	perror("execve");
	exit(EXIT_FAILURE);
}