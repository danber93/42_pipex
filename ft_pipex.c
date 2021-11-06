#include "ft_pipex.h"

#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

int	openfile(char *filename, int mode)
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

char	*getPath(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;
	
	i = 0;
	while(env[i] && ft_str_ncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_pchr(path, ':') > -1);
	{
		dir = ft_str_ncmp(path, ft_pchr(path, ':'));
		bin = path_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		path += ft_pchr(path, ':') + 1;
	}
	return (cmd);
}

void	exec(char *cmd, char **env)
{
	char	**args;
	char	*path;

	// preparazione del args
	args = ft_str_split(cmd, ' ');
	// se il primo argomento ha uno '/' vuol dire che é stato specificato e il path, e quindi non serve costruirselo
	if (ft_pchr(args[0], '/') > -1)
		path = args[0];
	else
		path = getPath(args[0], env);
	// eseguo il comando
	execve(path, args, env);
	write(STDERR, "pipex: ", 7);
	write(STDERR, cmd, ft_pchr(cmd, 0));
	write(STDERR, ": command not found\n", 20);
	exit(127);
}

void	redir(char *cmd, char **env, int fdin)
{
	pid_t	pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
	else
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT);
		// controllare se é necessario if (fdin == STDIN)
		exec(cmd, env);
	}
}

int		main(int ac, char **av, char **env)
{
	int	fdin;
	int	fdout;

	if (ac == 5)
	{
		fdin = openfile(av[1], INFILE);
		fdout  = openfile(av[4], OUTFILE);
  		dup2(fdin, STDIN);
		dup2(fdout, STDOUT);
		redir(av[2], env, fdin);
	}

}