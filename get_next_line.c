/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 16:33:49 by asyed             #+#    #+#             */
/*   Updated: 2017/10/10 19:46:34 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
# define FT_EOF 0x1A

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	while (n--)
		((char *)dest)[n] = ((const char *)src)[n];
	return (dest);
}

void	*ft_realloc(void *src, size_t new_size)
{
	void	*new;

	if (!src)
		return (malloc(new_size));
	if (!new_size)
	{
		free(src);
		return (NULL);
	}
	new = (void *)malloc(new_size);
	if (!new)
		return (NULL);
	new = ft_memcpy(new, src, new_size);
	free(src);
	return (new);
}

int		ft_endline(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n' || str[i] == FT_EOF)
			return (i);
		i++;
	}
	return (0);
}

void	ft_zerostring(char *str)
{
	while (*str)
		*str++ = '\0';
}

int		get_next_line(const int fd, char **line)
{
	int r;
	int count;
	int found;
	static	char **overflow;
	char	*save;
	int 	length;

	r = 0;
	count = 0;
	if (fd < 0 || !line)
		return (-1);
	if (!overflow)
	{
		overflow = (char **)malloc(FDS * sizeof(char *));
		if (!overflow)
			return (-1);
		bzero(overflow, FDS * sizeof(char *));
	}
	if (overflow[fd])
	{
		/*
		** ToDo: Handle Multiple in it.
		*/
		count += ft_endline(overflow[fd]);
		if (count)
		{
			// printf("Present (%d) = \"%s\"\n", count, overflow[fd]);
			*line = (char *)malloc((count + BUFF_SIZE + 1) * sizeof(char));
			if (!*line)
				return (-1);
			bzero(*line, (count + BUFF_SIZE + 1) * sizeof(char));
			ft_memcpy(*line, overflow[fd], count * sizeof(char));
			length = strlen(overflow[fd]);
			save = (char *)malloc((length - count) * sizeof(char));
			bzero(save, (length - count) * sizeof(char));
			ft_memcpy(save, &overflow[fd][count + 1], (length - count - 1) * sizeof(char));
			free(overflow[fd]);
			overflow[fd] = save;
			// printf("New overflow[%d] = \"%s\"\n", fd, overflow[fd]);
			// printf("Line = \"%s\"\n", *line);
			return (1);
		}
		else
		{
			count += strlen(overflow[fd]);
			*line = (char *)malloc((count + BUFF_SIZE + 1) * sizeof(char));
			if (!*line)
				return (-1);
			bzero(*line, (count + BUFF_SIZE + 1) * sizeof(char));
			*line = strcpy(*line, overflow[fd]);
			free(overflow[fd]);
			overflow[fd] = NULL;
		}
	}
	else
	{
		*line = (char *)malloc((BUFF_SIZE + 1) * sizeof(char));
		if (!*line)
			return (-1);
		bzero(*line, BUFF_SIZE + 1);
	}
	while ((r = read(fd, &(*line)[count], BUFF_SIZE)) > 0)
	{
		count += r;
		if ((found = ft_endline(*line)) > 0)
		{
			length = 0;
			if (overflow[fd])
			{
				length = strlen(overflow[fd]);
				overflow[fd] = (char *)ft_realloc(overflow[fd], (length + (count - found) * sizeof(char)));
				bzero(&overflow[fd][length], ((count - found) * sizeof(char)));
			}
			else
			{
				overflow[fd] = (char *)malloc((count - found) * sizeof(char));
				bzero(overflow[fd], (count - found) * sizeof(char));
			}
			ft_memcpy(&overflow[fd][length], &(*line)[found + 1], (count - found - 1) * sizeof(char));
			(*line)[found] = '\0';
			return (1);
		}
		if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc(*line, (count + BUFF_SIZE) * sizeof(char));
			bzero(&(*line)[count], BUFF_SIZE);
		}
	}
	if (r < 0)
		return (-1);
	(*line)[count] = '\0';
	return (**line ? 1 : 0);
}

// int				main(void)
// {
// 	char		*line;
// 	int			fd;
// 	int			ret;
// 	int			count_lines;
// 	char		*filename;
// 	int			errors;

// 	filename = "gnl3_3.txt";
// 	fd = open(filename, O_RDONLY);
// 	if (fd > 2)
// 	{
// 		count_lines = 0;
// 		errors = 0;
// 		line = NULL;
// 		while ((ret = get_next_line(fd, &line)) > 0)
// 		{
// 			printf("[Return] = \"%s\"\n", line);
// 			if (count_lines == 0 && strcmp(line, "123") != 0)
// 				errors++;
// 			if (count_lines == 1 && strcmp(line, "456") != 0)
// 				errors++;
// 			if (count_lines == 2 && strcmp(line, "789") != 0)
// 				errors++;
// 			if (count_lines == 3 && strcmp(line, "1012") != 0)
// 				errors++;
// 			if (count_lines == 4 && strcmp(line, "abcd") != 0)
// 				errors++;
// 			if (count_lines == 5 && strcmp(line, "ef") != 0)
// 				errors++;
// 			if (count_lines == 6 && strcmp(line, "qwertyuiopasdfg") != 0)
// 				errors++;
// 			count_lines++;
// 			if (count_lines > 50)
// 				break ;
// 		}
// 		close(fd);
// 		if (count_lines != 7)
// 			printf("-> must have returned '1' 7 times instead of %d time(s)\n", count_lines);
// 		if (errors > 0)
// 			printf("-> must have read \"1234567890abcde\" and \"fghijklmnopqrst\" and \"omgmahdude\"\n");
// 		if (count_lines == 7 && errors == 0)
// 			printf("OK\n");
// 	}
// 	else
// 		printf("An error occured while opening file %s\n", filename);
// 	return (0);
// }