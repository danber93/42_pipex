/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prulli <prulli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 10:15:53 by prulli            #+#    #+#             */
/*   Updated: 2021/12/02 14:32:21 by prulli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_pipex.h"

int	ft_str_ncmp(char *str1, char *str2, int n)
{
	while (--n > 0 && *str1 && *str2 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*str2 - *str1);
}

int	ft_str_cmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	while (str1[i] || str2[i])
	{
		if (!str1[i] || !str2[i] || (str1[i] != str2[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

char	*ft_str_ndup(char *str, unsigned int n)
{
	char				*duped;
	unsigned int		i;

	i = 0;
	duped = malloc(sizeof(char) * (n + 1));
	while (i < n)
		duped[i++] = *str++;
	duped[n] = 0;
	return (duped);
}

char	*ft_join_path(char *path, char *bin)
{
	char	*full_path;
	int		i;
	int		j;

	full_path = malloc(sizeof(char) * (ft_strchr(path, 0)
				+ ft_strchr(bin, 0) + 2));
	i = 0;
	j = 0;
	while (path[j])
		full_path[i++] = path[j++];
	full_path[i++] = '/';
	j = 0;
	while (bin[j])
		full_path[i++] = bin[j++];
	full_path[i] = 0;
	return (full_path);
}
