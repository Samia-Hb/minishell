/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 01:42:53 by szeroual          #+#    #+#             */
/*   Updated: 2024/12/03 11:56:39 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*readfile(int fd, char *str)
{
	char		*buff;
	ssize_t		readbytes;

	readbytes = 1;
	buff = ft_calloc((size_t)BUFFER_SIZE + 1, sizeof(char));
	if (!buff)
		return (NULL);
	while (readbytes != 0 && !ft_strchr(str, '\n'))
	{
		readbytes = read(fd, buff, BUFFER_SIZE);
		if (readbytes == -1)
		{
			free (str);
			free(buff);
			return (NULL);
		}
		buff[readbytes] = '\0';
		str = ft_strjoin(str, buff);
	}
	free(buff);
	return (str);
}

char	*extarct_line(char *buff)
{
	int		i;
	char	*line;

	i = 0;
	if (buff[i] == '\0')
		return (NULL);
	while (buff[i] && buff[i] != '\n')
		i++;
	if (buff[i] == '\n')
		i++;
	line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buff[i] && buff[i] != '\n')
	{
		line[i] = buff[i];
		i++;
	}
	if (buff[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*get_rest(char *save)
{
	int		i;
	char	*s;

	i = 0;
	if (!save)
		return (NULL);
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
	{
		free (save);
		return (NULL);
	}
	i++;
	s = ft_substr(save, i, ft_strlen(save) -i + 1);
	free(save);
	return (s);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*read_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	else
		read_line = readfile(fd, read_line);
	if (!read_line)
		return (NULL);
	line = extarct_line(read_line);
	read_line = get_rest(read_line);
	return (line);
}
