/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 16:33:49 by asyed             #+#    #+#             */
/*   Updated: 2017/10/20 20:38:40 by asyed            ###   ########.fr       */
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

int		line_split(char **overflow, char **line, int *count)
{
	int i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '\n')
		{
			if ((*line)[i + 1])
			{
				*overflow = (char *)ft_memalloc((*count - i) * SCHAR);
				*overflow = ft_strncpy(*overflow, &(*line)[i + 1], *count - i);
			}
			(*line)[i] = '\0';
			return (1);
		}
		i++;
	}
	return (0);
}

int		storage_contains(char **overflow, char **line)
{
	int		i;
	int		length;
	char	*save;

	i = 0;
	while ((*overflow)[i])
	{
		if ((*overflow)[i] == '\n')
		{
			length = ft_strlen(*overflow);
			save = ft_memalloc((length - i) * SCHAR);
			save = ft_strcpy(save, &(*overflow[i + 1]));
			*line = ft_strncpy(*line, *overflow, i);
			(*line)[i] = '\0';
			free(*overflow);
			*overflow = save;
			return (1);
		}
		i++;
	}
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static	char	**overflow;
	int				r;
	int				*count;
	int				res;

	count = ft_memalloc(sizeof(int));
	if (fd < 0 || !line)
		return (-1);
	if (!overflow)
		overflow = (char **)ft_memalloc(FDS * sizeof(char *));
	*line = (char *)ft_memalloc((BUFF_SIZE + 1) * SCHAR);
	if (overflow[fd] && (res = storage_contains(&(overflow[fd]), line)))
		return (res);
	while ((r = read(fd, &(*line)[*count], BUFF_SIZE)) > 0 && (*count += r))
		if ((res = line_split(&overflow[fd], line, count)))
			return (res);
		else if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc(*line, (*count + BUFF_SIZE + 1) * SCHAR);
			ft_bzero(&(*line)[*count], (BUFF_SIZE + 1) * SCHAR);
		}
	if (r < 0)
		return (-1);
	(*line)[*count] = '\0';
	return (**line ? 1 : 0);
}
