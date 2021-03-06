/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pformats.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcakmako tcakmako@student.42kocaeli.com.t  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 12:04:20 by tcakmako          #+#    #+#             */
/*   Updated: 2022/03/05 18:30:14 by tcakmako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdarg.h>
#include "ft_printf.h"

static char	check_set(char c, char *set)
{
	while (*set)
		if (c == *(set++))
			return (1);
	return (0);
}

static long	loc_set(char *s, char *set, size_t mem, int *width)
{
	*width = 0;
	while (s[mem])
	{
		if (ft_isdigit(s[mem]))
			*width = *width * 10 + (s[mem] - 48);
		else if (check_set(s[mem], set))
			return (mem);
		else if (*width)
			return (-1);
		mem++;
	}
	return (-1);
}

static char	*get_type(char *tn, va_list ap)
{
	if (!ft_tkncmp(tn, "p"))
		return (conv_hex(va_arg(ap, unsigned long long), 'p'));
	else if (!ft_tkncmp(tn, "s"))
		return (conv_str(va_arg(ap, char *)));
	else if (!ft_tkncmp(tn, "d") || !ft_tkncmp(tn, "i") || !ft_tkncmp(tn, "c"))
		return (conv_dec(va_arg(ap, int), *tn / 100));
	else if (!ft_tkncmp(tn, "u"))
		return (conv_dec(va_arg(ap, unsigned int), *tn / 100));
	else if (!ft_tkncmp(tn, "x") || !ft_tkncmp(tn, "X"))
		return (conv_hex(va_arg(ap, unsigned long long), *tn));
	else if (!ft_tkncmp(tn, "%"))
		return (ft_strdup("%"));
	return (NULL);
}

static char	*resizer(char *s, int width)
{
	size_t	ssize;
	size_t	scan;
	char	*news;

	ssize = ft_strlen(s);
	scan = 0;
	if ((size_t) width <= ssize)
		return (s);
	news = malloc(sizeof(char) * (width + 1));
	while (scan < width - ssize)
		news[scan++] = ' ';
	while (scan < (size_t) width)
		news[scan++] = *(s++);
	news[scan] = 0;
	free(s - ssize);
	return (news);
}

long	pformat(const char *input, long mem, char **store, va_list ap)
{
	long	end;
	int		width;
	char	*typeholder;

	if (input[mem] == '%')
		mem++;
	end = loc_set((char *) input, TYPE2TOKENS, mem, &width);
	if (end < 0)
		return (-1);
	typeholder = get_type((char *) &input[end], ap);
	if (!typeholder)
		return (-1);
	if (input[end] != '%')
		typeholder = resizer(typeholder, width);
	*store = ft_strffjoin(*store, typeholder);
	return (end + 1);
}
