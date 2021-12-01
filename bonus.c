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

char	*ft_get_path(char *cmd, char **env)
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
		path = ft_get_path(args[0], env);
	// eseguo il comando
	execve(path, args, env);
	write(STDERR, "pipex: ", 7);
	write(STDERR, cmd, ft_pchr(cmd, 0));
	write(STDERR, ": command not found\n", 20);
	exit(127);
}

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

// void	redir_heredoc(char *buff, char *cmd, char **env)
void	redir_heredoc(char *buff)
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
		write(pipefd[1], buff, ft_strlen(buff));
		exit(EXIT_SUCCESS);
	}
}

char	*ft_name_env(char *buff, int start_index, int len)
{
	int		i;
	char	*name;
	
	i = 0;
	name = ft_calloc(sizeof(char), len);
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
	value = ft_calloc(sizeof(char), 0);
	while (env[i])
	{
		tmp = ft_str_split(env[i], '=');
		if (ft_str_cmp(tmp[0], name))
		{
			value = ft_strdup(tmp[1]);
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
	dest = ft_calloc(sizeof(char), ft_strlen(buff) + diff);
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
	// dest[j] = '\0';
	free(buff);
	return dest;
}

char	*ft_replace(char *buff, char *env_val, int env_start, char *env_name) {
	int		i;
	int		j;
	int		k;
	char	*dest;
	int		diff;

	i = 0;
	diff = ft_strlen(env_name) - ft_strlen(env_val);
	dest = ft_calloc(sizeof(char), ft_strlen(buff) - diff);
	while (i < env_start)
	{
		dest[i] = buff[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(env_val))
		dest[i++] = env_val[j++];
	k = env_start + ft_strlen(env_name) + 1;
	while (buff[k])
		dest[i++] = buff[k++];
	free(buff);
	return dest;
}

char	*ft_extract_env_vars(char *buff, char **env)
{
	int		dollar_index;
	int		i;
	int		j;
	char	*name_env;
	char	*value_env;

	j = 0;
	i = 0;
	while (buff[j])
	{
		dollar_index = ft_pchr(buff, '$');
		if (dollar_index > -1 && buff[dollar_index + 1])
		{
			dollar_index++;
			while (buff[dollar_index + i] && !ft_is_space(buff[dollar_index + i]))
				i++;
			if (i > 0)
			{
				name_env = ft_name_env(buff, dollar_index, i);
				printf("name env = %s\n", name_env);
				value_env = ft_get_value_env(name_env, env);
				buff = ft_replace(buff, value_env, dollar_index - 1, name_env);
			}
			i = 0;
			j = dollar_index;
		}
		else
			j++;
	}
	return buff;
}

char *ft_remove_delimiter(char *buff, char *delimiter)
{
	char	*dest;
	int		delimiter_size;
	int		i;
	int		buff_size;

	buff_size = ft_strlen(buff);
	i = 0;
	delimiter_size = ft_strlen(delimiter);
	dest = ft_calloc(sizeof(char), (buff_size - delimiter_size));
	while (i < (buff_size - delimiter_size))
	{
		dest[i] = buff[i];
		i++;
	}
	free(buff);
	return dest;
}

void	ft_print_here_doc(int ac)
{
	int		i;
	char	*pipe_hd;
	char	*hd;

	pipe_hd = "pipe heredoc> ";
	hd = "heredoc> ";
	i = 0;
	if (ac >= 5)
		while (i < 15)
			write(1, &pipe_hd[i++], 1);
	else
		while (i < 9)
			write(1, &hd[i++], 1);
}

int		ft_is_delimiter(char *buff, char *del)
{
	int		del_len;
	char	*new_del;
	int		i = 0;

	del_len = ft_strlen(del);
	new_del = ft_calloc(sizeof(char), del_len + 2);
	while (i < del_len + 2)
	{
		if (i == 0)
			new_del[i] = '\n';
		else if (i == del_len + 1)
			new_del[i] = '\n';
		else 
			new_del[i] = del[i-1];
		i++;
	}
	if (ft_is_substr(buff, new_del))
		return (1);
	return (0);
}

void	ft_here_doc(int ac, char **av, char **env)
{
	ac = ac + 1;
	ac = ac - 1;
	// ------
	char	*buff;
	char	ch[2];
	int		i;
	int		fdout;
	// if (ac <= 5)
	// {
	// 	write(STDERR, "Here_doc. Invalid number  of arguments.\n", 29);
	// 	exit(1);
	// }
	buff = NULL;
	ft_print_here_doc(ac);
	while(read(STDIN, ch, 1))
	{
		buff = ft_str_rall(buff, ch[0]);
		if (ft_is_delimiter(buff, av[2]))
			break;
		if (ch[0] == '\n')
			ft_print_here_doc(ac);
	}
	buff = ft_extract_env_vars(buff, env);
	buff = ft_remove_delimiter(buff, av[2]);
	redir_heredoc(buff);
	fdout  = openfile(av[ac-1], OUTFILE);
	dup2(fdout, STDOUT);
	i = 3;
	while (i < ac - 2)
		redir(av[i++], env);
	exec(av[i], env);
}

int		main(int ac, char **av, char **env)
{
	int	fdin;
	int	fdout;
	int	i;

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