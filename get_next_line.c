/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 16:33:49 by asyed             #+#    #+#             */
/*   Updated: 2017/10/27 13:54:42 by asyed            ###   ########.fr       */
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

int		storage_contains(char **overflow, char **line, int *count)
{
	int		i;
	int		length;
	char	*save;

	i = 0;
	// printf("Whatever\n");
	// printf("Overflow (%p)\n", *overflow);
	// printf("(%p) Char was: \"%c\"\n", *overflow, (*overflow)[i]);
	while ((*overflow)[i])
	{
		// printf("Entered with overflow[%d] = %c\n", i, (*overflow)[i]);
		if ((*overflow)[i] == '\n')
		{
			// printf("New Line detected.\n");
			length = ft_strlen(*overflow);
			save = ft_memalloc((length - i) * SCHAR);
			save = ft_strcpy(save, &((*overflow)[i + 1]));
			// save = ft_strcpy(save, (*overflow + i));
			// printf("wow length - i = %d\n", length - i);
			// printf("save = \"%s\"\n", save);
			*line = ft_strncpy(*line, *overflow, i);
			// printf("Line = \"%s\" and Save = \"%s\"\n", *line, save);
			// printf("Char at [%d] is \"%c\"\n", i, save[i - 2]);
			(*line)[i] = '\0';
			free(*overflow);
			*overflow = save;
			// printf("Overflow is now set to %p\n", *overflow);
			return (1);
		}
		// printf("Didn't pass [%d]\n", i);
		i++;
	}
	// printf("Got broken out.\n");
	// printf("Char was: (%d) \"%c\"\n", i, (*overflow)[i]);
	*count += i;
	*line = ft_strncpy(*line, *overflow, i);
	// printf("Line = \"%s\"\n", *line);
	*line = (char *)ft_realloc(*line, (*count + BUFF_SIZE + 1) * SCHAR);
	ft_bzero((*line + *count), BUFF_SIZE * SCHAR);
	// printf("Line = \"%s\"\n", *line);
	free(*overflow);
	// printf("Free'd\n");
	*overflow = NULL;
	// printf("Set it to NULL\n");
	return (0);
}

int 	line_split(char **overflow, char **line, int *count)
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
				*overflow = ft_strncpy(*overflow, (*line + i + 1), *count - i);
				(*overflow)[*count - i - 1] = '\0';
			}
			(*line)[i] = '\0';
			return (1);
		}
		i++;
	}
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static 	char 	**overflow;
	int 			r;
	int				*count;
	int				res;

	count = ft_memalloc(sizeof(int));
	if (fd < 0 || !line)
		return (-1);
	if (!overflow)
		overflow = (char **)ft_memalloc(FDS * sizeof(char *));
	*line = (char *)ft_memalloc((BUFF_SIZE + 1) * SCHAR);
	if (overflow[fd])
	{
		if ((res = storage_contains(&(overflow[fd]), line, count)))
			return (res);
	}
	while ((r = read(fd, (*line + *count), BUFF_SIZE)) > 0)
	{
		*count += r;
		(*line)[*count] = '\0';
		if ((res = line_split(&(overflow[fd]), line, count)))
			return (res);
		// else
			// printf("No new line found.\n");
		if (r == BUFF_SIZE)
		{
			*line = (char *)ft_realloc(*line, (*count + BUFF_SIZE + 1) * SCHAR);
			ft_bzero((*line + *count), (BUFF_SIZE + 1) * SCHAR);
		}
	}
	if (r < 0)
		return (-1);
	(*line)[*count] = '\0';
	return (**line ? 1 : 0);
}
