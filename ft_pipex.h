/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbertill <dbertill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 21:16:44 by dbertill          #+#    #+#             */
/*   Updated: 2021/12/06 21:16:44 by dbertill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PIPEX_H
# define FT_PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define INFILE 0
# define OUTFILE 1

int		ft_str_ncmp(char *str1, char *str2, int n);
int		ft_strchr(char *str, char c);
char	*ft_str_ndup(char *str, unsigned int n);
char	*ft_join_path(char *path, char *bin);
char	**ft_str_split(char *str, char div);
int		ft_str_cmp(char *str1, char *str2);
char	*ft_realloc(char *buff, char ch);
int		ft_is_substr(char *str1, char *str2);
char	*ft_extract_env_vars(char *buff, char **env);
int		ft_strlen(char *str);
char	*ft_calloc(size_t size, size_t count);
char	*ft_strdup(char *s1);
size_t	ft_is_space(char c);
char	*ft_extract_env_vars(char *buff, char **env);
char	*ft_remove_delimiter(char *buff, char *delimiter);
void	ft_print_here_doc(int ac);
void	ft_redir_heredoc(char *buff);
char	*ft_name_env(char *buff, int start_index, int len);
char	*ft_get_value_env(char	*name, char **env);
char	*ft_env_filling(char *buff, char *env_val, int env_start, int diff);
char	*ft_replace(char *buff, char *env_val, int env_start, char *env_name);
int		ft_is_delimiter(char *buff, char *del);
char	*ft_get_path(char *cmd, char **env);
int		ft_is_substr(char *str, char *substr);
char	*ft_realloc(char *buff, char ch);
char	*ft_calloc(size_t size, size_t count);

#endif