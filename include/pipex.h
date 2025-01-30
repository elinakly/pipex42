/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:17:26 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/30 20:08:13 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>

// typedef struct s_command
// {
// 	char	*cmd;
// 	char	*file_in;
// 	char	*file_out;
// }			t_command;

void		execute(char *command, char **envp);
int			error(int status);

// t_command	command_first(char *cmd, char *file_in);
// t_command	command_middle(char *cmd);
// t_command	command_last(char *cmd, char *file_out);
#endif