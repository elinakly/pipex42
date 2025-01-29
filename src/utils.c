/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:23 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/29 19:35:54 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	*find_valid_path(char *com, char **envp)
{
	int		i;
	char 	**paths;
	char 	*path;
	char 	*find_full_path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		error();
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		error();
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		if (!paths)
		{
			free_arr(paths);
			error();
		}
		find_full_path = ft_strjoin(path, com);
		free(path);
		if (!find_full_path)
		{
			free_arr(paths);
			error();
		}
		if (access(find_full_path, F_OK) == 0)
			return (free_arr(paths), find_full_path);
		free(find_full_path);
		i++;
	}
	return (free_arr(paths), NULL);
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
		free_arr(command);
		error();
	}
	if (execve(find_path, command, envp) == -1)
	{
		free(com);
		free_arr(command);
		error();
	}
}
