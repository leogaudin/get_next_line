/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 17:30:18 by lgaudin           #+#    #+#             */
/*   Updated: 2023/05/04 02:42:40 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

char	*append_to_stash(char *stash, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(stash, buffer);
	free(stash);
	return (temp);
}

char	*read_to_buffer(int fd, char *stash)
{
	int		read_bytes;
	char	*buffer;

	if (!stash)
		stash = ft_calloc(1, sizeof(char));
	read_bytes = 1;
	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(stash);
			free(buffer);
			return (0);
		}
		buffer[read_bytes] = '\0';
		stash = append_to_stash(stash, buffer);
		if (ft_strchr(buffer, '\n') != 0)
			break ;
	}
	free(buffer);
	return (stash);
}

char	*get_new_stash(char *stash, int nl_index)
{
	char	*new_stash;
	int		i;

	if (stash[nl_index] == '\0')
	{
		free(stash);
		return (0);
	}
	new_stash = ft_calloc(ft_strlen(stash) - nl_index + 1, sizeof(char));
	nl_index++;
	i = 0;
	while (stash[nl_index])
	{
		new_stash[i] = stash[nl_index];
		i++;
		nl_index++;
	}
	free(stash);
	return (new_stash);
}

char	*stash_to_line(char *stash)
{
	char	*line;
	int		nl_index;
	int		i;

	if (stash[0] == '\0')
		return (0);
	nl_index = get_nl_index(stash);
	line = ft_calloc(nl_index + 2, sizeof(char));
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i] = stash[i];
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (!(fd >= 0 && BUFFER_SIZE > 0))
	{
		free(stash);
		return (0);
	}
	if (read(fd, 0, 0) < 0)
	{
		free(stash);
		return (0);
	}
	stash = read_to_buffer(fd, stash);
	if (!stash)
	{
		return (0);
	}
	line = stash_to_line(stash);
	stash = get_new_stash(stash, get_nl_index(stash));
	return (line);
}

// int	main(void)
// {
// 	// static char	*stash;
// 	// char		*buffer;
// 	// char		*line;
// 	// stash = "Salut je m'appelle Bruno\nJ'aime les pâtes sa mère";
// 	// buffer = "Salut";
// 	// line = stash_to_line(&stash);
// 	// printf("%s\n", line);
// 	for (int i = 0; i < 10; i++)
// 		printf("%s\n", get_next_line(open("test.txt", O_RDONLY)));
// 	return (0);
// }
