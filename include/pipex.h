/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:17:26 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/28 18:51:15 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
# include "libft.h"
# include "get_next_line.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>


int		execute(char *command, char **envp);
void	error(void);

# endif