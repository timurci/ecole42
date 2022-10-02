/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademirci <ademirci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 20:27:07 by ademirci          #+#    #+#             */
/*   Updated: 2022/09/26 23:37:59 by ademirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "shell.h"
# include "lexer.h"
# include "parser.h"
# include "expander.h"
# include "builtins.h"
# include "signals.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

//path_returner.c
char	*path_returner(char *cmd);

//executor.c
int		executor(char *line, t_shell *shell);

//exec_loop.c
void	exec_loop(t_shell *shell);

//utils.c
int		is_wait(t_cmd cmd);
int		operator_or(t_shell *shell, int i);
int		operator_and(t_shell *shell, int i);
void	wait_list(t_shell *shell, int last);
int		count_proccess(t_cmd *cmds);
int		command_check(t_shell *shell, int i);
int		error_check(t_shell *shell, int i);

//exec_builtins.c
int	is_builtin(t_cmd *cmd);
int	is_assignment(t_cmd *cmd);
int	exec_builtin(t_shell *shell, t_cmd *cmd);
#endif
