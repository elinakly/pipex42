/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/02/20 14:01:50 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "ft_printf.h"
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>

typedef struct s_command
{
	char	*cmd;
	char	*file_in;
	char	*file_out;
}			t_command;

void		execute(char *com, char **envp);
int			error(int status);
void		create_pipes(int num_cmds, int **pipes);
void		free_arr(char **arr);

t_command	command_first(char *cmd, char *file_in);
t_command	command_middle(char *cmd);
t_command	command_last(char *cmd, char *file_out);
#endif