/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcakmako <tcakmako@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:30:07 by tcakmako          #+#    #+#             */
/*   Updated: 2022/09/12 14:35:19 by tcakmako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/* extra skip can cause flaws in skip_quote() or split_tokens() */

static char	*skip_quote(char *input)
{
	unsigned char	quote;

	quote = *input;
	input++;
	while (*input && *input != quote)
		input++;
	return (input);
}

static char	*skip_all_quotes(char *input)
{
	input = skip_quote(input);
	if (*input)
		input++;
	if (*input == '\'' || *input == '\"')
		input = skip_all_quotes(input);
	return (input);
}

static char	*find_special(char *input)
{
	char	*init;

	init = input;
	if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, "<<", 2))
		input += 2;
	else if (*input == '<' || *input == '>')
		input++;
	else if (!ft_strncmp(input, "&&", 2) || !ft_strncmp(input, "||", 2))
		input += 2;
	else if (*input == '|')
		input++;
	if (init != input)
		return (input);
	return (NULL);
}

static int	count_tokens(char *input)
{
	int		count;

	count = 0;
	if (!ft_isspace(*input) && *input != '\'' && *input != '\"')
		count++;
	if (!ft_isspace(*input) && *input != '\'' && *input != '\"')
		input++;
	while (*input)
	{
		count++;
		if (*input == '\"' || *input == '\'')
			input = skip_all_quotes(input) - 1;
		else if (find_special(input))
		{
			if (*find_special(input) && !ft_isspace(*find_special(input)))
				count++;
		}
		else if (!ft_isspace(*input) && ft_isspace(*(input - 1)))
			;
		else
			count--;
		if (*input)
			input++;
	}
	return (count);
}

char	**split_tokens(char *input)
{
	int		itr;
	char	*check_p;
	char	**strs;

	strs = (char **) malloc(sizeof(*strs) * (count_tokens(input) + 1));
	itr = 0;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		check_p = input;
		while (*input && !ft_isspace(*input) && !find_special(input))
		{
			if (*input == '\"' || *input == '\'')
				input = skip_all_quotes(input);
			else if (*input && !ft_isspace(*input))
				input++;
		}
		if (check_p == input && find_special(input))
			input = find_special(input);
		strs[itr++] = ft_substr(check_p, 0, input - check_p);
	}
	strs[itr] = NULL;
	return (strs);
}