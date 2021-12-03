/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prulli <prulli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 18:16:55 by prulli            #+#    #+#             */
/*   Updated: 2021/12/03 18:16:56 by prulli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

int	ft_is_delimiter(char *buff, char *del)
{
	int		del_len;
	char	*new_del;
	int		i;

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
		return (1);
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
