/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 16:33:49 by asyed             #+#    #+#             */
/*   Updated: 2017/10/20 12:40:23 by asyed            ###   ########.fr       */
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
#define FT_EOF 0x1A
#define CHAR sizeof(char)

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

int		empty_check(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\n')
			return (0);
		i++;
	}
	return (i ? 1 : 0);
}

int		storage_copy(char **line, char **storage, int *count)
{
	char	*save;
	int		length;
 
	*count = ft_endline(*storage);
	if (*count)
	{
		*line = (char *)ft_memalloc((*count + BUFF_SIZE + 1) * CHAR);
		if (!*line)
			return (-1);
		ft_memcpy(*line, *storage, *count * CHAR);
		length = ft_strlen(*storage);
		save = (char *)ft_memalloc((length - *count) * CHAR);
		ft_memcpy(save, &(*storage)[*count + 1], (length - *count - 1) * CHAR);
		free(*storage);
		*storage = save;
		return (1);
	}
	*count = ft_strlen(*storage);
	*line = (char *)ft_memalloc((*count + BUFF_SIZE + 1) * CHAR);
	if (!*line)
		return (-1);
	*line = ft_strcpy(*line, *storage);
	free(*storage);
	*storage = NULL;
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	int				r;
	int				*count;
	int				found;
	static	char	**overflow;
	int				length;

	printf("\n============\nEntered New loop\n============\n");

	count = ft_memalloc(sizeof(int));
	if (fd < 0 || !line)
		return (-1);
	if (!overflow)
		if (!(overflow = (char **)ft_memalloc(FDS * sizeof(char *))))
			return (-1);
	if (overflow[fd])
	{
		if ((r = storage_copy(line, &overflow[fd], count)))
			return (r);
	}
	else
	{
		if (!(*line = (char *)ft_memalloc((BUFF_SIZE + 1) * CHAR)))
			return (-1);
	}
	printf("String in func = \"%s\"\n", *line);
	while ((r = read(fd, &(*line)[*count], BUFF_SIZE)) > 0)
	{
		*count += r;
		if ((found = ft_endline(*line)) > 0)
		{
			length = 0;
			if (overflow[fd])
			{
				length = ft_strlen(overflow[fd]);
				overflow[fd] = (char *)ft_realloc(overflow[fd], (length +
								(*count - found) * CHAR));
				ft_bzero(&overflow[fd][length], ((*count - found) * CHAR));
			}
			else
				overflow[fd] = (char *)ft_memalloc((*count - found) * CHAR);
			ft_memcpy(&overflow[fd][length], &(*line)[found + 1],
						(*count - found - 1) * CHAR);
			(*line)[found] = '\0';
			printf("Returned here: Str = [\nReturned string\n\"%s\"\n]\n", *line);
			return (1);
		}
		if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc(*line, (*count + BUFF_SIZE) * CHAR);
			ft_bzero(&(*line)[*count], BUFF_SIZE);
		}
	}
	if (r < 0)
		return (-1);
	if (empty_check(*line))
	{
		(*line)[*count - 1] = '\0';
		return (1);
	}
	(*line)[*count] = '\0';
	return (**line ? 1 : 0);
}
