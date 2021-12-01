#include "ft_pipex.h"

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
	return (name);
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
			return (value);
		}
		i++;
	}
	return (value);
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
	free(buff);
	return (dest);
}

char	*ft_replace(char *buff, char *env_val, int env_start, char *env_name)
{
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
	return (dest);
}
