/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:49:23 by eklymova          #+#    #+#             */
/*   Updated: 2025/01/30 19:43:35 by eklymova         ###   ########.fr       */
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

static char	*find_valid_path(const char *com, char **envp)
{
	int		i;
	char	**paths;
	char	path[PATH_MAX];

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		ft_strlcpy(path, paths[i], PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, com, PATH_MAX);
		if (access(path, F_OK) == 0)
			return (free_arr(paths), ft_strdup(path));
		i++;
	}
	return (free_arr(paths), ft_strdup(com));
}

void	execute(char *com, char **envp)
{
	char	*find_path;
	char	**command;

	command = ft_split(com, ' ');
	if (!command)
		exit(error(3));
	find_path = find_valid_path(command[0], envp); //malloc
	if (find_path == NULL)
	{
		free_arr(command);
		exit(error(3));
	}
	if (execve(find_path, command, envp) == -1)
	{
		free(find_path);
		free_arr(command);
		exit(error(127));
	}
}
