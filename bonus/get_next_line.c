/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbettal <hbettal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 13:51:07 by hbettal           #+#    #+#             */
/*   Updated: 2024/02/17 19:18:35 by hbettal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strchr(char *s, int c)
{
	unsigned int	i;
	char			chr;

	i = 0;
	chr = (char)c;
	while (s[i])
	{
		if (s[i] == chr)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == chr)
		return ((char *)&s[i]);
	return (NULL);
}

char	*edit_left(char *line)
{
	char	*left;
	size_t	i;

	if (!line)
		return (NULL);
	i = 0;
	while (line[i] != '\n' && line[i])
		i++;
	left = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!left)
		return (free (left), free (line), NULL);
	if (line[i])
		line[i + 1] = '\0';
	return (left);
}

char	*newline_finder(int fd, char *left, char *str)
{
	ssize_t	bytes;
	char	*tmp;

	bytes = 1;
	while (!ft_strchr(str, '\n'))
	{
		bytes = read(fd, str, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		str[bytes] = '\0';
		if (left)
		{
			tmp = left;
			left = ft_strjoin(left, str);
			free (tmp);
		}
		else
			left = ft_strdup(str);
	}
	free(str);
	return (left);
}

char	*get_next_line(int fd)
{
	static char	*left;
	char		*line;
	char		*str;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (free(left), left = NULL);
	str = malloc(BUFFER_SIZE + 1);
	if (!str)
		return (free(left), left = NULL);
	str[0] = 0;
	line = newline_finder(fd, left, str);
	if (!line)
		return (NULL);
	left = edit_left(line);
	if (!line[0])
		return (free(left), free(line), left = NULL);
	return (line);
}
