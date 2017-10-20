/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asyed <asyed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/08 16:33:57 by asyed             #+#    #+#             */
/*   Updated: 2017/10/19 12:35:45 by asyed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <strings.h>
# define BUFF_SIZE 30
# define FDS 256

int				get_next_line(const int fd, char **line);
void			*ft_memalloc(size_t size);
void			*ft_realloc(void *src, size_t new_size);
void			*ft_memcpy(void *dest, const void *src, size_t n);
size_t			ft_strlen(const char *s);
char			*ft_strcpy(char *dest, const char *src);
void			ft_bzero(void *s, size_t n);

#endif