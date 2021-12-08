/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:49:52 by dbertill          #+#    #+#             */
/*   Updated: 2021/12/08 17:32:13 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

/* First row is ONLY delimiter, then return TRUE, else FALSE */
int	ft_first_row_del(char *buff, char *del)
{
	int	i;

	i = 0;
	while (del[i])
	{
		if (buff[i] != del[i])
			return (0);
		i++;
	}
	return (1);
}

int	ft_is_delimiter(char *buff, char *del)
{
	int		del_len;
	char	*new_del;
	int		i;

	if (ft_first_row_del(buff, del))
		return (1);
	del_len = ft_strlen(del);
	new_del = ft_calloc(sizeof(char), del_len + 2);
	i = 0;
	while (i < del_len + 2)
	{
		if (i == 0)
			new_del[i] = '\n';
		else if (i == del_len + 1)
			new_del[i] = '\n';
		else
			new_del[i] = del[i - 1];
		i++;
	}
	if (ft_is_substr(buff, new_del))
	{
		free(new_del);
		return (1);
	}
	free(new_del);
	return (0);
}

char	*ft_get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_str_ncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_strchr(path, ':') > -1)
	{
		dir = ft_str_ndup(path, ft_strchr(path, ':'));
		bin = ft_join_path(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_strchr(path, ':') + 1;
	}
	return (cmd);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	size_t	len;
	size_t	i;

	len = ft_strlen(src);
	dest = ft_calloc(sizeof(char), len);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}
