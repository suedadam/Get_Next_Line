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
	if (!overflow)
	{
		overflow = (char **)malloc(FDS * sizeof(char *));
		if (!overflow)
			return (-1);
		bzero(overflow, FDS * sizeof(char *));
	}
	if (overflow[fd])
	{
		count += ft_endline(overflow[fd]);
		if (count)
		{
			*line = (char *)malloc((count + BUFF_SIZE + 1) * sizeof(char));
			if (!*line)
				return (-1);
			bzero(*line, (count + BUFF_SIZE + 1) * sizeof(char));
			save = (char *)malloc((strlen(overflow[fd]) - count + 1) * sizeof(char));
			if (!save)
				return (-1);
			bzero(save, (strlen(overflow[fd]) - count + 1) * sizeof(char));
			save = strcpy(save, &overflow[fd][count]);
			*line = strcpy(*line, overflow[fd]);
			free(overflow[fd]);
			overflow[fd] = save;
			count += strlen(*line);
		}
		else
		{
			*line = (char *)malloc((BUFF_SIZE + 1) * sizeof(char));
			if (!*line)
				return (-1);
			bzero(*line, (BUFF_SIZE + 1) * sizeof(char));
			*line = strcpy(*line, overflow[fd]);
			free(overflow[fd]);
			overflow[fd] = NULL;
			count += strlen(*line);
		}
	}
	else
	{
		*line = (char *)malloc((BUFF_SIZE + 1) * sizeof(char));
		if (!*line)
			return (-1);
		ft_zerostring(*line);
	}
	while ((r = read(fd, &(*line)[count], BUFF_SIZE)) > 0)
	{
		count += r;
		// printf("[While] \"%s\"\n", *line);
		if ((found = ft_endline(*line)) > 0)
		{
			length = 0;
			if (overflow[fd])
			{
				length = strlen(overflow[fd]);
				overflow[fd] = (char *)ft_realloc(overflow[fd], (length + (count - found)) * sizeof(char));
			}
			else
			{
				overflow[fd] = (char *)malloc((count - found) * sizeof(char));
			}
			strcpy(&overflow[fd][length], &(*line)[found + 1]);
			(*line)[found] = '\0';
			return (1);
		}
		if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc(*line, (count + BUFF_SIZE) * sizeof(char));
		}
	}
	if (r < 0)
		return (-1);
	(*line)[count] = '\0';
	return (**line ? 1 : 0);
}

int  main(void)
{
	char		*line;
	int			fd;
	int			ret;
	int			count_lines;
	char		*filename;
	int			errors;

	filename = "gnl3_3.txt";
	fd = open(filename, O_RDONLY);
	if (fd > 2)
	{
		count_lines = 0;
		errors = 0;
		line = NULL;
		while ((ret = get_next_line(fd, &line)) > 0)
			printf("[Return]: \"%s\"\n", line);
		close(fd);
	}
	else
		printf("An error occured while opening file %s\n", filename);
	return (0);
}