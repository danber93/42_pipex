#include "ft_pipex.h"

#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

int openfile(char *filename, int mode)
{
	if (mode == 0)
	{
		if (access(filename, F_OK))
		{
			write(2, "File does not exist.\n", 7);
			return (0);
		}
		return (open(filename, O_RDONLY));
	}
	else
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
}

void redir (char *cmd, char **env, int fdin)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
}

int		main(int ac, char **av, char **env)
{
	int	fdin;
	int	fdout;

	if (ac == 5)
	{
		fdin = openfile(av[1], INFILE);
		fdout  = openfile(av[4], OUTFILE);
		printf("fdin= %i\n", fdin);
		// replace standard input with input file
  		dup2(fdin, STDIN);
		// replace standard output with output file
		dup2(fdout, STDOUT);
		redir(av[2], env, fdin);
		printf("fdin= %i\n", fdin);
	}
}