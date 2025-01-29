/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:17:26 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/29 19:33:16 by eklymova         ###   ########.fr       */
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


void		execute(char *command, char **envp);
void		error(void);

# endif