#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h> //remove later

typedef struct s_ints
{
	int		*arr;
	size_t	inv;
	size_t	max;
}			t_ints;

t_ints	*dup_ints(t_ints *a);
void	delete_ints(t_ints *list);

void	push(t_ints *a, t_ints *b, char *mode);
void	swap(t_ints *l, char *mode);
void	rot(t_ints *l, char *mode);
void	rrot(t_ints *l, char *mode);

#endif