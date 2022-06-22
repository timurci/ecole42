/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcakmako <tcakmako@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 12:35:28 by tcakmako          #+#    #+#             */
/*   Updated: 2022/06/22 12:35:29 by tcakmako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static t_colors	*add_palette(int bg, int fg, t_colors *head)
{
	t_colors	*newp;

	newp = malloc(sizeof(*newp));
	newp->bg = bg;
	newp->fg = fg;
	newp->next = NULL;
	if (head)
		head->next = newp;
	return (newp);
}

t_colors	*init_color_palette(void)
{
	t_colors	*first;
	t_colors	*pivot;

	first = add_palette(0x00000000, 0x00FFFFFF, NULL);
	pivot = add_palette(0x00FFFFFF, 0x00000000, first);
	pivot->next = first;
	return (first);
}
