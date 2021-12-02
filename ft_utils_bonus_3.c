/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:47:39 by dbertill          #+#    #+#             */
/*   Updated: 2021/12/02 12:47:40 by dbertill         ###   ########.fr       */
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
