/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:34:41 by eklymova          #+#    #+#             */
/*   Updated: 2025/02/21 19:06:03 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	error(int status)
{
	if (status == 1)
		perror("Dup2 failed");
	else if (status == 2)
		perror("Path failed");
	else if (status == 3)
		perror("Execve failed");
	else if (status == 126)
		ft_putstr_fd("Error: command not found\n", 2);
	else if (status == 127)
		ft_putstr_fd("Execve failed\n", 2);
	else if (status == 4)
		ft_putstr_fd("Error: open failed\n", 2);
	exit (status);
}

void	create_pipes(int num_cmds, int **pipes)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	free_arr(char **arr, int **pipes, int num_cmds, int status)
{
	int	i;

	if (status == 0)
	{
		i = 0;
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
		free(arr);
	}
	if (status == 1)
	{
		i = 0;
		while (i < num_cmds - 1)
		{
			free(pipes[i]);
			i++;
		}
		free(pipes);
	}
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
		if (access(path, X_OK) == 0)
			return (free_arr(paths, 0, 0, 0), ft_strdup(path));
		i++;
	}
	return (free_arr(paths, 0, 0, 0), NULL);
}

void	execute(char *com, char **envp)
{
	char	*find_path;
	char	**command;

	command = ft_split(com, ' ');
	if (!command)
		exit(error(3));
	find_path = find_valid_path(command[0], envp);
	if (command[0] == NULL)
	{
		free_arr(command, 0, 0, 0);
		exit(error(3));
	}
	if (find_path == NULL)
	{
		free_arr(command, 0, 0, 0);
		exit(error(127));
	}
	if (execve(find_path, command, envp) == -1)
	{
		free(find_path);
		free_arr(command, 0, 0, 0);
		exit(error(126));
	}
}
