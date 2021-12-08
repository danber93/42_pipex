/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 20:49:24 by dbertill          #+#    #+#             */
/*   Updated: 2021/12/06 20:49:26 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

/* Check if substr is contained in str */
int	ft_is_substr(char *str, char *substr)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (substr[j] && str[i + j] == substr[j])
		{
				j++;
		}
		if (!substr[j])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_realloc(char *buff, char ch)
{
	char	*new;
	int		len;
	int		i;

	if (buff == NULL)
	{
		buff = ft_calloc(sizeof(char), 1);
		buff[0] = ch;
		return (buff);
	}
	len = ft_strlen(buff);
	new = ft_calloc(sizeof(char), len + 1);
	i = 0;
	while (i < len)
	{
		new[i] = buff[i];
		i++;
	}
	new[i] = ch;
	free(buff);
	return (new);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_calloc(size_t size, size_t count)
{
	char	*new;
	size_t	i;

	new = malloc(size * (count + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < count)
		new[i++] = '\0';
	new[i] = 0;
	return (new);
}

char	**ft_str_split(char *str, char div)
{
	char	**matrix;
	int		count;
	int		i;
	int		j;

	count = 0;
	j = 0;
	while (str[j])
	{
		if (str[j++] == div)
			count++;
	}
	matrix = malloc(sizeof(char *) * (count + 2));
	matrix[count + 1] = NULL;
	i = 0;
	while (i < count + 1)
	{
		j = 0;
		while (str[j] && str[j] != div)
			j++;
		matrix[i++] = ft_str_ndup(str, j);
		str = str + j + 1;
	}
	return (matrix);
}
