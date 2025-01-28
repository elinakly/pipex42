/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:23 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/28 20:02:58 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	find_valid_path(char *com, char **envp)
{
	
}

void	free_command(char **command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		free(command[i]);
		i++;
	}
	free(command);
	error();
}

void	execute(char *com, char **envp)
{
	char	*find_path;
	char	**command;

	command = ft_split(com, ' ');
	if (!command)
	{
		free(com);
		error();
	}
	find_path = find_valid_path(command[0], envp);
	if (!find_path)
	{
		free(com);
		free_command(command);
		error();
	}
	if (execve(find_path, command, envp) == -1)
	{
		free(com);
		free_command(command);
		error();
	}
}
