/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prulli <prulli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 10:16:02 by dbertill          #+#    #+#             */
/*   Updated: 2021/12/02 14:31:10 by prulli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

size_t	ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r'
		|| c == '\f');
}

char	*ft_replace_var(char *buff, char **env, int i, int d_index)
{
	char	*name_env;
	char	*value_env;

	while (buff[d_index + i] && !ft_is_space(buff[d_index + i]))
		i++;
	if (i > 0)
	{
		name_env = ft_name_env(buff, d_index, i);
		value_env = ft_get_value_env(name_env, env);
		buff = ft_replace(buff, value_env, d_index - 1, name_env);
	}
	return (buff);
}

char	*ft_extract_env_vars(char *buff, char **env)
{
	int		d_index;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (buff[j])
	{
		d_index = ft_strchr(buff, '$');
		if (d_index > -1 && buff[d_index + 1])
		{
			d_index++;
			buff = ft_replace_var(buff, env, i, d_index);
			i = 0;
			j = d_index;
		}
		else
			j++;
	}
	return (buff);
}

char	*ft_remove_delimiter(char *buff, char *delimiter)
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
	return (dest);
}

void	ft_print_here_doc(int ac)
{
	int		i;
	char	*pipe_hd;
	char	*hd;

	pipe_hd = "pipe heredoc> ";
	hd = "heredoc> ";
	i = 0;
	if (ac >= 4)
		while (i < 15)
			write(1, &pipe_hd[i++], 1);
	else
		while (i < 9)
			write(1, &hd[i++], 1);
}
