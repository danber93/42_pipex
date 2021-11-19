#include "ft_pipex.h"

#include <stdio.h>

int		openfile(char *filename, int mode)
{
	if (mode == 0)
	{
		if (access(filename, F_OK))
		{
			write(STDERR, "pipex: ", 7);
			write(STDERR, filename, ft_pchr(filename, 0));
			write(2, "File does not exist.\n", 7);
			return (STDIN);
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
	while (path && ft_pchr(path, ':') > -1)
	{
		dir = ft_str_ndup(path, ft_pchr(path, ':'));
		bin = path_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
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

// void	redir(char *cmd, char **env, int fdin)
void	redir(char *cmd, char **env)
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

void	ft_here_doc(int ac, char **av, char **env)
{
	// Questi solo per compilare, vanno eliminati
	av[0] = "4";
	ac = ac + 1;
	// ------
	char	*buff;
	char	ch[2];

	// if (ac <= 5)
	// {
	// 	write(STDERR, "Here_doc. Invalid number  of arguments.\n", 29);
	// 	exit(1);
	// }
	buff = NULL;
	printf("av[2] = %s\n", av[2]);
	while(read(STDIN, ch, 1))
	{
		buff = ft_str_rall(buff, ch[0]);
		if (ft_is_substr(buff, av[2]))
			break;
	}
	printf("Ho letto fino alla stringa di DELIMITER. buff = %s\n", buff);
	ft_extract_env_vars(buff, env);
}

size_t	ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r'
		|| c == '\f');
}

char	*ft_name_env(char *buff, int start_index, int len)
{
	int		i;
	char	*name;
	
	i = 0;
	name = ft_calloc(len);
	if (!name)
		return (NULL);
	while (i < len)
	{
		name[i] = buff[start_index + i];
		i++;
	}
	return name;
}

char	*ft_get_value_env(char	*name, char **env)
{
	char	**tmp;
	char	*value;
	int		i;

	i = 0;
	value = ft_calloc(0);
	while (env[i])
	{
		tmp = ft_str_split(env[i], "=");
		if (!ft_str_cmp(tmp[0], name))
		{
			value = ft_str_dup(tmp[1]);
			return value;
		}
		i++;
	}
	return value;

}

char	*ft_env_filling(char *buff, char *env_val, int env_start, int diff)
{
	int		i;
	int		j;
	int		k;
	char	*dest;

	i = 0;
	j = 0;
	dest = ft_calloc(ft_strlen(buff) + diff + 1);
	while (buff[i] && i < env_start)
		dest[j++] = buff[i++];
	if (i == env_start)
	{
		k = 0;
		while (k < ft_strlen(env_val))
			dest[j++] = env_val[k++];
		i = i + k - diff;
		while (buff[i])
			dest[j++] = buff[i++];
	}
	dest[j] = '\0';
	return dest;
}

char	*ft_extract_env_vars(char *buff, char **env)
{
	int		dollar_index;
	int		i;
	char	*name_env;
	char	*value_env;

	i = 0;
	dollar_index = ft_pchr(buff, "$");
	if (dollar_index > -1 && buff[dollar_index + 1])
	{
		while (buff[dollar_index + i] && !ft_is_space(buff[dollar_index + i]))
			i++;
		if (i > 0)
		{
			name_env = ft_name_env(buff, dollar_index + 1, i);
			value_env = ft_get_value_env(name_env, env);
			// a questo punto dobbiamo ri mallocare il buff e inserire il valore della variabile d'ambiente
			buff = ft_env_filling(
				buff, value_env,
				i, ft_strlen(value_env) - ft_strlen(name_env) - 1);// -1 per includere il dollaro da cancellare
		}
		
	}
}

int		main(int ac, char **av, char **env)
{
	int	fdin;
	int	fdout;
	int	i;
	printf("env[0] = %s", env[0]);

	i = 2;
	if (av[1] && ft_str_cmp(av[1], "here_doc"))
		ft_here_doc(ac, av, env);
	else if (ac >= 5)
	{
		fdin = openfile(av[1], INFILE);
		fdout  = openfile(av[ac-1], OUTFILE);
  		dup2(fdin, STDIN);
		dup2(fdout, STDOUT);
		while (i < ac - 2)
			redir(av[i++], env);
		exec(av[i], env);
	}
	else
		write(STDERR, "Invalid number  of arguments.\n", 29);
	return (1);
}